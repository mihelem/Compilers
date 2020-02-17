<?php

define("DEBUGGAMAN", false);

class Path {
	public const BINDIR = "/var/www/bin/";
	public const DATADIR = "/var/www/data/";
	public const BUILDDIR = "/var/www/generated/";
	public const INCLUDEDIR = "/var/www/include/";
	public const LIBDIR = "/var/www/lib/";
	public const SOURCEDIR = "/var/www/src/";
	public const UPLOADDIR = "/var/www/uploads/";
}

$request_page = "regexp.php";
$regexp_query = "regexp";
$graphs = [
	'mdfa' => 'Minimal DFA ',
	'searchmdfa' => 'Minimal Search Automaton '];
$opt_per_graph = [
	'mermaid' => 'Graph of ',
	'gotocode' => 'GOTO-style code of ',
	'match' => 'Find (in File) Matches of '];
$opts_trig_response_on_upload = ['match'];
