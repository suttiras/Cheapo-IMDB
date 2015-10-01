<html>
<head><title>Our Amazing Calculator!</title></head>

<body>
<h1><font size="5" color="red" face="Arial Black">Our Amazing Calculator!</h1></font>
<font size="3" color="black">Please enter a valid arithmetic expression into the following input box.</font>
</br>
<b><font size="3" color="black">Note: </b>Parentheses are not supported in this application.</font>
</br>

<FORM METHOD = "GET">
	<INPUT TYPE="text" NAME="inputexpr">
	<INPUT TYPE="submit" VALUE="Enter">
	</br></br><b><font size="4" color="black">Result</font></b>
</br>
	<?php
			if ($_GET["inputexpr"] != "")
			{
				$subject = str_replace(" ", "", $_GET["inputexpr"]);
				//echo $subject;
			
				//check invalid characters
				if (!preg_match('/^[\d\s\.\-\+\/\*]+$/', $subject))
				{
					echo "Invalid input expression!";
				}
				//check if there are digits in between decimals
			
				else if (preg_match("/[0-9]*[\.][0-9]*[\.]/", $subject))
					echo "Invalid input expression!";
			
				else 
				{
					preg_match("/([-]?([0-9]+)[\/\.\+\*-]?)*([-]?[0-9])+/", $subject, $matchedpattern);
					
					if (strlen($subject) == strlen($matchedpattern[0]))
						{
							//new for --
							$newequation = str_replace("--","+",$matchedpattern[0]);
							//
							//echo $matchedpattern[0];
							//eval("\$result = $matchedpattern[0];");
							eval("\$result = $newequation;");
							//echo $result;
							//echo "</br>";
							
							if (is_numeric($result))
							{
								echo $subject;
								echo " = ";
								echo $result;
							}
							else	
								echo "Invalid input expression!";
						}
					else
						echo "Invalid input expression!";
					
				}
			}
			
	?>
</FORM>
</html>
