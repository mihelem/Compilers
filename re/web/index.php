<?php
if (session_status() == PHP_SESSION_NONE) {
    session_start();
}

if (!isset($_SESSION['nonce']) || empty($_SESSION['nonce'])) {
	$_SESSION['nonce'] = $_SERVER['REMOTE_ADDR'].$_SERVER['REQUEST_TIME']; //bin2hex(random_bytes(32));
}

require_once "commons.php";
?>

<!DOCTYPE html>
<html>
<head>
	<title>RegExp</title>
	<link rel="stylesheet" href="./css/index.css">

	<script src="https://unpkg.com/mermaid@8.4.6/dist/mermaid.min.js"></script>
	<script>
	mermaid.initialize({
     		startOnLoad:false
 	});

 	var settings = {
 		request_page : <?php echo json_encode($request_page); ?>,
 		regexp_query : <?php echo json_encode($regexp_query); ?>,
 		graphs : <?php echo json_encode(array_keys($graphs)); ?>,
 		opt_per_graph : <?php echo json_encode(array_keys($opt_per_graph)); ?>,
 		opts_trig_response_on_upload : <?php echo json_encode($opts_trig_response_on_upload) ?>,
 		new_regexp : 0
 	};

 	function updateResponseOnUpload() {
		var opts = getOptions();
		var result = false;

		settings.opts_trig_response_on_upload.forEach(
			function (opt) {
				settings.graphs.forEach(
					function (graph) {
						result = result || (opts[graph][opt] == 1);
					})
			});
		return result;
 	}

 	function getOptions() {
 		var options = {};

 		settings.graphs.forEach(
 			function (graph) {
 				options[graph] = {};
 				settings.opt_per_graph.forEach (
 					function (opt) {
 						options[graph][opt] = 
 							document.getElementById(graph+'_'+opt).checked == true ? 1 : 0;
 					}
 				);
 			}
 		);

 		options['regexp_changed'] = settings.new_regexp;

 		return options;
 	}

 	function getQueryFromOptions(opts) {
		var query = [];

 		settings.graphs.forEach(
 			function (graph) {
 				settings.opt_per_graph.forEach (
 					function (opt) {
 						query.push(graph+'_'+opt+'='+opts[graph][opt]);
 					}
 				);
 			}
 		);

 		query.push('regexp_changed'+'='+opts['regexp_changed']);

		return query.join('&');
 	}

 	function isOptionCheckedAnywhere(opts, opt) {
 		if (typeof opt === 'undefined') {
	 		for(let graph of settings.graphs) {
	 			for(let opt_ of settings.opt_per_graph) {
	 				if (opts[graph][opt_]) {
	 					return true;
	 				}
	 			}
	 		}
	 	} else {
	 		for(let graph of settings.graphs) {
	 			if ((opt in opts[graph]) && opts[graph][opt] === 1) {
	 				return true;
	 			}
	 		}
	 	}
	 	return false;
 	}

 	function parseNewRegexp() {
 		settings.new_regexp = 1;
 		parseRegexp();
 		settings.new_regexp = 0;
 	}


	function parseRegexp() {
		var regexp = document.getElementById("regexp").value;
		const options = getOptions();

		if (regexp.length === 0) { //|| isOptionCheckedAnywhere(options) === false) {
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

	function toggleTextUploadForm() {
		document.getElementById('text_upload_id').classList.toggle('hidden');
	}

	function handleOpt(graph, opt) {
		if (settings.opts_trig_response_on_upload.includes(opt)) {
			textElement = document.getElementById('text_upload_id');
			if (updateResponseOnUpload()) {
				//textElement.classList.remove('hidden');
			} else {
				//textElement.classList.add('hidden');
			}
		}
	}
	</script>
</head>
<body>
<div class="user_input">
<form>
	<input
		type="textarea"
		onkeyup="parseNewRegexp()"
		id="regexp"
		maxlength="-1"
		autofocus /><br />
	<div class="user_options">
	<table style="width:100%">
		<?php
			foreach ($graphs as $graph => $graph_desc) {
				echo "<tr>";
				foreach ($opt_per_graph as $opt => $opt_desc) {
					echo "<td><input type=\"checkbox\" id=\"", $graph, '_', $opt, "\" onclick=\"parseRegexp(); handleOpt('",$graph,"', '",$opt,"')\">",
					$opt_desc, $graph_desc, "</td>\n";
				}
				echo "</tr>";
			}
		?>
	</table>
	</div>
</form>
</div>
<div class="program_output">
<p><span id="resultSpan"></span></p>
</div>
<div class="text_upload_container">
<div class="text_upload_friend" id="temp"></div>
<div class="text_upload align_right" id="text_upload_id">
<table><tr>
<td>
	<div class="uploaded_file_name hidden" id="uploaded_file_name_id"></div>
</td>
<td>
	<progress id="progress_bar_id" class="progress_bar hidden" />
</td>
<td>
	<label for="upload_file_input_id" >
		<img src="./images/upload.png" id="upload_file_icon_id" class="upload_file_icon"/>
	</label>
	<input type="file" id="upload_file_input_id" class="hidden" name="text_upload_file" />
</td>
</tr>
</table>
<script>
	var uploadedFilename = "";

	var inputForm = document.getElementById('upload_file_input_id');
	inputForm.oninput = function (event) {
		var result = document.getElementById("resultSpan");

		var action = 'upload.php';
		var progressBar = document.getElementById('progress_bar_id');

		var formData = new FormData();

		var file = document.getElementById('upload_file_input_id').files[0];
		var filename = document.getElementById('uploaded_file_name_id');


		if (file) {
			var xmlhttp = new XMLHttpRequest();

			xmlhttp.upload.onprogress = function(event) {
				if (event.lengthComputable) {
					progressBar.max = event.total;
					progressBar.value = event.loaded;
					progressBar.opacity = 1- event.loaded/event.total;
					filename.style.opacity = event.loaded/event.total;
				}
			}
			xmlhttp.upload.onloadstart = function(event) {
				progressBar.value = 0;
				progressBar.style.opacity = 1;
				progressBar.classList.remove('hidden');
				filename.classList.remove('hidden');
			}
			xmlhttp.upload.onloadend = function(event) {
				progressBar.value = event.loaded;
				progressBar.classList.add('hidden');
				filename.opacity = 1;
			}

			xmlhttp.onreadystatechange = function(event) {
				var status, text, readyState;

				try {
					readyState = event.target.readyState;
					text = event.target.responseText;
					status = event.target.status;
				} catch(e) {
					return;
				}

				if (readyState == 4 && status == '200') {
					if (updateResponseOnUpload()) {
						parseRegexp();
					}	
				}
			}

			filename.innerHTML = file.name;
			formData.append('text_upload_file', file);
			xmlhttp.open('POST', action, true);
			xmlhttp.send(formData);
		}
	}
</script>

</div>
</div>
<br /><br /><br />
</body>
</html>
