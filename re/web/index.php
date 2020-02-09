<?php
session_start();

if (!isset($_SESSION['nonce']) || empty($_SESSION['nonce'])) {
	$_SESSION['nonce'] = bin2hex(random_bytes(32));
}

require_once "commons.php";
?>

<!DOCTYPE html>
<html>
<head>
	<link rel="stylesheet" href="./css/prova.css">

	<script src="./js/node_modules/mermaid/dist/mermaid.js"></script>
	<script>
	mermaid.initialize({
     		startOnLoad:false
 	});

 	var settings = {
 		request_page : <?php echo json_encode($request_page); ?>,
 		regexp_query : <?php echo json_encode($regexp_query); ?>,
 		graphs : <?php echo json_encode(array_keys($graphs)); ?>,
 		opt_per_graph : <?php echo json_encode(array_keys($opt_per_graph)); ?>,
 	};

 	function getOptions() {
 		var options = {};

 		settings.graphs.forEach(
 			function (graph) {
 				options[graph] = {};
 				settings.opt_per_graph.forEach (
 					function (opt) {
 						options[graph][opt] = 0;
 					}
 				);
 			}
 		);

 		return options;
 	}

 	function setOptions(opts) {
 		for (var graph in opts) {
 			for (var opt in opts[graph]) {
 				opts[graph][opt] = 
 					document.getElementById(graph+'_'+opt).checked == true ? 1 : 0;
 			}
 		}

 		return opts;
 	}

 	function getQueryFromOptions(opts) {
		var query = [];

		for (var graph in opts) {
			for (var opt in opts[graph]) {
				query.push(graph+'_'+opt+'='+opts[graph][opt]);
			}
		}

		return query.join('&');
 	}

 	function isOptionCheckedAnywhere(opts, opt) {
 		for (var graph in opts) {
 			 if (typeof opt === 'undefined') {
 				for (var _opt in opts[graph]) {
 					if (opts[graph][_opt] === 1) {
 						return true;
 					}
 				}
 			} else if ((opt in opts[graph]) && opts[graph][opt] === 1) {
 				return true;
 			}
 		}

 		return false;
 	}


	function parseRegexp() {
		var regexp = document.getElementById("regexp").value;
		const options = setOptions(getOptions());

		if (regexp.length === 0 || isOptionCheckedAnywhere(options) === false) {
			document.getElementById("resultSpan").innerHTML = "";
			return;
		} else {
			var xmlhttp = new XMLHttpRequest();
			xmlhttp.onreadystatechange = function() {
					if (this.readyState == 4 && this.status == 200) {
						document.getElementById("resultSpan").innerHTML =
							this.responseText;
						if (isOptionCheckedAnywhere(options, 'mermaid')) {
							mermaid.init();
						}
					}
				};

			var query =
				settings.request_page+'?'
				+ settings.regexp_query+'='+encodeURIComponent(regexp)+'&'
				+ getQueryFromOptions(options);
			xmlhttp.open("GET", query, true);
			xmlhttp.send();
		}
	}
	</script>
</head>
<body>
<form>
	RegExp:
	<input
		type="text"
		onkeyup="parseRegexp()"
		id="regexp" size="120"
		maxlength="10000"
		autofocus><br />
	<table>
		<?php
			foreach ($graphs as $graph => $graph_desc) {
				echo "<tr>";
				foreach ($opt_per_graph as $opt => $opt_desc) {
					echo "<td><input type=\"checkbox\" id=\"", $graph, '_', $opt, "\" onclick=\"parseRegexp()\">",
					$opt_desc, $graph_desc, "</td>\n";
				}
				echo "</tr>";
			}
		?>
	</table>
</form>
<p><span id="resultSpan"></span></p>
</body>
</html>
