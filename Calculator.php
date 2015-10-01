<html>
<head><title>Our Amazing Calculator!</title></head>

<body>
<h1><font size="3" color="red" face="Arial Black">Our Amazing Calculator!</h1></font>
<font size="1" color="black">Please enter a valid arithmetic expression into the following input box.</font>
</br>

<FORM METHOD = "GET">
	<INPUT TYPE="text" NAME="inputexpr">
	<INPUT TYPE="submit" VALUE="Enter">
	<?php
	
			//echo "Hello World!";
			if ($_GET["inputexpr"] != "")
			{
				$subject = str_replace(" ", "", $_GET["inputexpr"]);
			
				//check invalid characters
			
				if (!preg_match('/^[\d\s\.\-\+\/\*]+$/', $subject))
				{
					echo "Invalid input expression! ";
					echo $subject;
				}
			
				//check if there are digits in between decimals
			
				else if (preg_match("/[0-9]*[\.][0-9]*[\.]/", $subject))
					echo "Invalid input expression! There is an issue with your decimal. $subject";
				
				else {
				preg_match("/([-]?([0-9]+)[\/\.\+\*-]?)*([-]?[0-9])+/", $subject, $matchedpattern);
				//echo "Going to evaluate expression! </br>";
				//echo "matchedpattern is $matchedpattern[0]";
				echo '</br>';
				echo "subject is $subject";
				if (strlen($subject) == strlen($matchedpattern[0]))
				{
					//echo "</br> matchedpattern and subject are the same!";
					eval("\$result = $matchedpattern[0];");
					if (is_numeric($result))
						echo "</br> Result is: $result";
					else	
						echo "Invalid input expression!";
				}
				}
			/*
				else 
				{
					//preg_match("/([-]?([0-9]+)[\/\.\+\*-]?)*([-]?[0-9])+/", $subject, $matchedpattern);
					if (strlen($subject) == strlen($matchedpattern[0]))
						{
						
							eval("\$result = \"$matchedpattern[[0]";");
							if (is_numeric($result))
								echo $result;
							else	
								echo "Invalid input expression!";
								
								echo "Going to evaluate expression!"
						}
					else
						echo "Invalid input expression!";
					
				}
			
			*/
			
			}
	?>
</FORM>
