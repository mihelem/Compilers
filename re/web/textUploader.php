<?php
if (session_status() == PHP_SESSION_NONE) {
    session_start();
}

require_once("commons.php");

class TextUploader {
	const TARGET_DIR = Path::UPLOADDIR;
	const MAX_SIZE = 1000000;
	
	protected $errorMessage;
	protected $sourceFileName;
	protected $targetFileName;
	
	function __construct(
		$source_file_name,
		$target_file_name) 
	{
		$this->sourceFileName = $source_file_name;
		$this->targetFileName = $target_file_name;
		$this->errorMessage = "";
	}

	protected function checkAndMove() {
		if ($_FILES[$this->sourceFileName]['size'] > static::MAX_SIZE) {
			$this->errorMessage = "Max size exceeded.";
			return false;
		}
		if (
			move_uploaded_file(
				$_FILES[$this->sourceFileName]["tmp_name"],
				$this->targetFileName)) 
		{
			return true;
		}
		$this->errorMessage = "An error occurred while moving the temporary file";
		return false;
	}

	public function upload() {
		if ($this->checkAndMove() === false) {
			echo $this->errorMessage;
		}
		$_SESSION['uploaded'][] = $this->targetFileName;
	}

	static function getTargetFromSource ($source_file_name) {
		return static::TARGET_DIR . $source_file_name . $_SESSION['nonce'];
	}

	public static function get($file_name) {
		return
			new static(
				$file_name,
				static::getTargetFromSource($file_name));
	}
}