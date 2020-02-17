<?php
if (session_status() == PHP_SESSION_NONE) {
    session_start();
}

require_once "textUploader.php";

$uploader = TextUploader::get("text_upload_file");
$uploader->upload();