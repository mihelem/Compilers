<?php
session_start();

require_once "commons.php";

final class RegExp {
	const REGEXPFILE = "./data/regexp";
	const REGEXPARSER = "./bin/scangen";
	const SEPARATOR = "~~~~~~~~~~\n";

	protected $regexp;
	protected $nonce;
	protected $options;
	protected $roptions;
	protected $filename;

	protected $mermaidMinimalDFA = "";
	protected $gotoCodeMinimalDFA = "";


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
		$this->filename = self::REGEXPFILE.$this->nonce;
	}

	public function saveInput() {
		$handle = fopen($this->filename, "wb");
		fwrite($handle, $this->regexp."\n");
		fclose($handle);

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
		$command = self::REGEXPARSER.' '.implode(' ', $this->getParserArgs())." < ".$this->filename;
		$parser_output =
			explode(
				self::SEPARATOR,
				shell_exec($command));

		foreach ($parser_output as $i => $output) {
			$id = strstr($output, "\n", true);
			if ($id) {
				$this->options[$this->roptions[$id]['graph']][$this->roptions[$id]['opt']]['body'] =
					strstr($output, "\n");
			}
		}

		return $this;
	}

	protected function renderBody(
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
		} else {	// TODO: prepare search (upload text file etc...)
			return $this->options[$graph][$opt]['body'];
		}
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

		$response = "<table>";
		foreach ($options as $graph => $opts) {
			$response .= "<tr>";
			foreach ($opts as $opt_name => $opt) {
				if ($opt['active'] == 1) {
					$response .=
						"<td>".$reg_exp->renderBody($graph, $opt_name)."</td>\n";
				}
			}
			$response .= "</tr>\n";
		}
		$response .= "</table>\n";

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
echo RegExp::renderResponse(
		$_REQUEST[$regexp_query],
		$options,
		$roptions
	);
