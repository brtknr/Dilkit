<?php
class database {
	private function open ($host, $username, $password, $database){
	@mysql_connect($host, $username, $password)
	    or die ('MySQL Connection Error: ' . mysql_error());

	mysql_select_db($database) or die('MySQL Database Selection Error: '.$database);
	}

	function query ($query){
	$this->open("localhost","dilkit","password","dilkit");

	$result = @mysql_query($query) or die('MySQL Query Error: ' . mysql_error());

	while ($row = @mysql_fetch_assoc($result)){
		$rows[] = $row;
	}

	@mysql_free_result($result);

	return $rows;
	}
	
	function rawQuery ($query){
		$result = @mysql_query($query) or die('MySQL Query Error: ' . mysql_error());
		return $result;
	}
}
?>