<?php
if (session_status() == PHP_SESSION_NONE) {
    session_start();
}

require_once "commons.php";
require_once "textUploader.php";

final class RegExp {
	const REGEXFILE = Path::DATADIR."regexp";
	const REGEXPARSER = Path::BINDIR."scangen";
	const SEPARATOR = "~~~~~~~~~~\n";

	protected $regexp;
	protected $nonce;
	protected $options;
	protected $roptions;
	protected $filename;

	function __construct(
		$regexp,
		$nonce,
		$options,
		$roptions)
	{
		$this->regexp = $regexp;
		$this->nonce = $nonce;
		$this->options = $options;
		$this->roptions = $roptions;
		$this->filename = self::REGEXFILE.$this->nonce;
	}

	public static function writeFile($filename, $content) {
		$handle = fopen($filename, 'wb');
		if (is_resource($handle)) {
			fwrite($handle, $content);
			fclose($handle);

			return true;
		}

		return false;
	}

	public function saveInput() {
		static::writeFile($this->filename, $this->regexp."\n");

		return $this;
	}

	protected function getParserArgs() {
		$args = [];
		foreach ($this->options as $graph => $opts) {
			foreach ($opts as $opt_name => $opt) {
				if ($opt['active'] == 1) {
					$args[] = $opt['id'];
				}
			}
		}

		return $args;
	}

	public function parse() {
		$command = self::REGEXPARSER.' '.implode(' ', $this->getParserArgs());
		$descriptor = [
			["pipe", "r"],
			["pipe", "w"]];
		$process = proc_open($command, $descriptor, $pipes);
		if (is_resource($process)) {
			fwrite($pipes[0], $this->filename."\n");
			fclose($pipes[0]);

			$parser_output = explode(
				self::SEPARATOR,
				stream_get_contents($pipes[1]));
			fclose($pipes[1]);
			proc_close($process);

			foreach ($parser_output as $i => $output) {
				$id = strstr($output, "\n", true);
				if ($id == "error") {
					$this->options["error"]["body"] = strstr($output, "\n");
				} else if ($id) {
					$this->options[$this->roptions[$id]['graph']][$this->roptions[$id]['opt']]['body'] =
						strstr($output, "\n");
				}
			}
		}

		return $this;
	}

	protected function getRenderError() {
		return 
			(array_key_exists("error", $this->options) 
			&& array_key_exists("body",$this->options["error"]))
			? $this->options["error"]["body"]
			: "";
	}

	protected function getRenderBody(
		$graph,
		$opt)
	{
		$id = $this->options[$graph][$opt]['id'];
		if ($opt == 'mermaid') {
			return
				"<div id=\"".$id."\" class=\"mermaid\">"
					.$this->options[$graph][$opt]['body']
				."</div>";
		} elseif ($opt == 'gotocode') {
			return
				"<div id=\"".$id."\"><pre><code>"
 					.$this->options[$graph][$opt]['body']
 				."</code></pre>";
		} elseif ($opt == 'match') {
			if (!array_key_exists('uploaded', $_SESSION) ||
				!in_array(
					TextUploader::getTargetFromSource('text_upload_file'),
					$_SESSION['uploaded']))
			{
				return 
					"<div id=\"".$id."\"><pre><code>"
						.$this->options[$graph][$opt]['body']
					."</code></pre>";
			} else {
				$source_prefix = $graph.'_'.$opt;
				if ( !array_key_exists('built', $_SESSION) ||
					 !array_key_exists($graph, $_SESSION['built']) ||
					 $_SESSION['built'][$graph] != $_SESSION['regexp_id'])
				{
					$this->buildCode($source_prefix, $this->options[$graph][$opt]['body']);
					$_SESSION['built'][$graph] = $_SESSION['regexp_id'];
				}
				$program = static::getBuildName($source_prefix);
				$result = shell_exec(
					$program." ".TextUploader::getTargetFromSource('text_upload_file'));

				return 
					"<div id=\"".$id."\"><pre><code>"
						.$result
					."</code></pre>";
			}
		}
	}

	public function getRenderResponse($options, $roptions) {
		$response = $this->getRenderError();
		if (!$response) {
			$response = "<table>";
			foreach ($options as $graph => $opts) {
				$response .= "<tr>";
				foreach ($opts as $opt_name => $opt) {
					if ($opt['active'] == 1) {
						$response .=
							"<td>".$this->getRenderBody($graph, $opt_name)."</td>\n";
					}
				}
				$response .= "</tr>\n";
			}
			$response .= "</table>\n";
		}

		return $response;
	}

	public static function getBuildName($name_prefix) {
		return Path::BUILDDIR.$name_prefix.$_SESSION['nonce'];
	}

	protected function buildCode($name_prefix, $code) {
		$source_name = static::getBuildName($name_prefix);
		static::writeFile($source_name.'.c', $code);
		shell_exec(
			"cc -c ".$source_name.".c -o ".$source_name.".o -I".Path::INCLUDEDIR
			." && "."cc ".$source_name.".o ".Path::LIBDIR."string_t.o -o ".$source_name."\n");

		return $source_name;
	}

	public static function renderResponse(
		$regexp,
		$options,
		$roptions)
	{
		$reg_exp = new RegExp(
			$regexp,
			$_SESSION["nonce"],
			$options,
			$roptions);
		$reg_exp->saveInput();
		$reg_exp->parse();

		$response = $reg_exp->getRenderResponse($options, $roptions);

		return
			utf8_encode($response."\n");
	}
}

$options = [];
$roptions = [];
foreach ($graphs as $graph => $graph_desc) {
	foreach ($opt_per_graph as $opt => $opt_desc) {
		$options[$graph][$opt] = [
			'id' => $graph.'_'.$opt,
			'active' => $_REQUEST[$graph.'_'.$opt],
			'desc' => $opt_desc.$graph_desc,
			'body' => "" ];
		$roptions[$graph.'_'.$opt] = [
			'graph' => $graph,
			'opt' => $opt
		];
	}
}

if (($_REQUEST['regexp_changed'] == 1)) {
	$_SESSION['regexp_id'] = bin2hex(random_bytes(32));
}

echo RegExp::renderResponse(
		$_REQUEST[$regexp_query],
		$options,
		$roptions
	);

if (DEBUGGAMAN == true) {
	echo "<hr><br>";
	print_r($_REQUEST);
	echo "<br><br><pre>";
	print_r($_SESSION);
	echo "</pre>";
}