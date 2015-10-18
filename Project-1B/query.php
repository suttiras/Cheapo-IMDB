<html>
<head><title>CS143 Project 1B</title></head>
<body>
<p><b>CS143 Project 1B</b></p>
<p>Please do not run a complex query here. You may kill the server. </p>
Type an SQL query in the following box: <p>
Example: <tt>SELECT * FROM Actor WHERE id=10;</tt><br />
<p>
<form action="." method="GET">
<textarea name="query" cols="60" rows="8"></textarea><br />
<input type="submit" value="Submit" />

<?php
$db_connection = mysql_connect("localhost", "cs143", "");
mysql_select_db("TEST", $db_connection);

mysql_close($db_connection);
?>
</form>
</p>
<p><small>Note: tables and fields are case sensitive. All tables in Project 1B are available.</small>
</p>


</body>
</html>
