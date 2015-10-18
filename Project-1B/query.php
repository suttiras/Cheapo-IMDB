<html>
<head><title>CS143 Project 1B</title></head>
<body>
<p><b>CS143 Project 1B</b></p>
<p>Please do not run a complex query here. You may kill the server. </p>
Type an SQL query in the following box: <p>
Example: <tt>SELECT * FROM Actor WHERE id=10;</tt><br />
<p>
<form action="" method="GET">
<textarea name="query" cols="60" rows="8"></textarea><br />
<input type="submit" value="Submit" />
<?php
if ($_GET["query"] != "")
{
	echo "<br><br><b>Your query is: </b></br> </br>";
	echo $_GET["query"];
	echo "<br><br>";
	$db_connection = mysql_connect("localhost", "cs143", "");
	
	mysql_select_db("TEST", $db_connection);
	
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
</form>

</p>
<p><small>Note: tables and fields are case sensitive. All tables in Project 1B are available.</small>
</p>


</body>
</html>
