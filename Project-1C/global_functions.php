<?php

function add_to_db_actor($first_name, $last_name, $gender, $dob, $dod)
{
		$db_connection = mysql_connect("localhost", "cs143", "");
		mysql_select_db("CS143", $db_connection);
		
	
	
	//$sanitized_name = mysql_real_escape_string($query, $db_connection);
	$rs = mysql_query($_GET["query"], $db_connection);
	
	if (!$rs) {
		echo "<br><br>Could not successfully run query from DB: " . mysql_error();
		exit;
	}

	if (mysql_num_rows($rs) == 0) {
		echo "<br><br>No rows found.";
		exit;
	}

	$result = mysql_fetch_assoc($rs);
	
	echo "<table border = 1 cellspacing = 1><tr>";
	
	//echo $result;
	
	foreach (array_keys($result) as $col)
	{
		echo "<th>";
		echo $col;
		echo "</th>";
	}
	
	echo "</tr>";

	echo "<tr>";
	
	do
	{
		foreach($result as $col)
		{
			echo "<td>";
			echo $col; 
			echo "</td>";
		}
		echo "</tr>";
	}while ($result = mysql_fetch_assoc($rs));
	
	echo "</table>";
	
	mysql_free_result($result);
	mysql_close($db_connection);

}
?>