<html>
<head><title>Our Amazing Calculator!</title></head>

<body>
<<<<<<< HEAD
<h1><font size="5" color="red" face="Arial Black">Our Amazing Calculator!</h1></font>
<font size="3" color="black">Please enter a valid arithmetic expression into the following input box.</font>
</br>
<b><font size="3" color="black">Note: </b>Parentheses are not supported in this application.</font>
=======
<h1><font size="3" color="red" face="Arial Black">Our Amazing Calculator!</h1></font>
<font size="1" color="black">Please enter a valid arithmetic expression into the following input box.</font>
>>>>>>> origin/master
</br>

<FORM METHOD = "GET">
	<INPUT TYPE="text" NAME="inputexpr">
	<INPUT TYPE="submit" VALUE="Enter">
<<<<<<< HEAD
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
=======
	<?php
	
			//echo "Hello World!";
			if ($_GET["inputexpr"] != "")
			{
				$subject = str_replace(" ", "", $_GET["inputexpr"]);
				
				//check invalid characters
			
				if (!preg_match('/^[\d\s\.\-\+\/\*]+$/', $subject))
				{
					echo "</br>Invalid input expression! There is an invalid character.";
					echo $subject;
				}
			
				//check if there are digits in between decimals
			
				else if (preg_match("/[0-9]*[\.][0-9]*[\.]/", $subject))
					echo "</br>Invalid input expression! There is an issue with your decimal. $subject";
				
				else {
				preg_match("/([-]?([0-9]+)[\/\.\+\*-]?)*([-]?[0-9])+/", $subject, $matchedpattern);
				//echo "Going to evaluate expression! </br>";
				//echo "matchedpattern is $matchedpattern[0]";
				echo '</br>';
				//echo "subject is $subject";
				if (strlen($subject) == strlen($matchedpattern[0]))
				{
					//echo "</br> matchedpattern and subject are the same!";
					$evaluate = str_replace("--", "+", $matchedpattern[0]);
					
					//eval("\$result = $matchedpattern[0];");
					eval("\$result = $evaluate;");
					if (is_numeric($result))
						echo "</br> $subject = $result";
					else	
						echo "</br>Invalid input expression!";
				}
				else
				{
					echo "</br>Invalid input expression!";
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
>>>>>>> origin/master
						}
					else
						echo "Invalid input expression!";
					
				}
<<<<<<< HEAD
			}
			
	?>
</FORM>
</html>
=======
			
			*/
			
			}
	?>
</FORM>
>>>>>>> origin/master
