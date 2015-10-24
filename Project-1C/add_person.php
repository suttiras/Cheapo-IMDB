<html>
<head>
	<title>IMDB: Cheapo Version</title>
</head>
<style>
.error {color: #FF0000;}
</style>
<body>
<p style="text-align:center"><a href="./Homepage.php"><img src="ImdbLogo.png" alt="Website Logo"></a></p>

<p>
Actor/Director Page
</p>

<p>

<?php
// define variables and set to empty values
$firstnameErr = $lastnameErr = $genderErr = $dobErr = "";
$first_name = $last_name = $gender = $dob = $dod = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
   if (empty($_POST["first_name"])) {
     $firstnameErr = "First Name is required";
   } else {
     $first_name = test_input($_POST["first_name"]);
     // check if name only contains letters and whitespace
     if (!preg_match("/^[a-zA-Z ]*$/",$first_name) {
       $firstnameErr = "Only letters and white space allowed"; 
     }
   }
   if (empty($_POST["last_name"])) {
     $lastnameErr = "Last Name is required";
   } else {
     $last_name = test_input($_POST["last_name"]);
     // check if name only contains letters and whitespace
     if (!preg_match("/^[a-zA-Z ]*$/",$last_name)) {
       $lastnameErr = "Only letters and white space allowed"; 
     }
   }

   if (empty($_POST["dob"])) {
     $dobErr = "Need a date of birth!";
   } else {
     $dob = test_input($_POST["dob"]);
   }

   if (empty($_POST["gender"])) {
     $genderErr = "Gender is required";
   } 
   else {
     $gender = test_input($_POST["gender"]);
   }
}
?>

<form method="GET" action=""> 
   First Name: <input type="text" name="first_name">
   <span class="error">* <?php echo $firstnameErr;?></span>
   <br><br>
   
   Last Name: <input type="text" name="last_name">
	<span class="error">* <?php echo $lastnameErr;?></span>
   <br><br>
   Gender:
   <input type="radio" name="gender" value='female'>Female
   <input type="radio" name="gender" value='male'>Male
   <span class="error">* <?php echo $genderErr;?></span>
   <br><br>
   Date of Birth: <input type="text" name="dob">
	<span class="error">* <?php echo $dobErr;?></span>
   <br><br>
   Date of Death (if applicable): <input type="text" name="dod">
   <br><br>

   <br><br>
   <input type="submit" name="submit" value="Submit"> 
</form>


</p>


<h2 style="text-align:center"><font size = "3"><b>Add to Database</b></font></h2>
<p style="text-align:center">
Actor/Director
</p>
<p style="text-align:center">
<a href="./add_movie.php">Movie</a>
</p>
<p style="text-align:center">
<a href="./add_comments.php">Reviews</a>
</p>
<p style="text-align:center">
<a href="./actor_to_movie.php">Add an Actor to a Movie</a>
</p>
<p style="text-align:center">
<a href="./director_to_movie.php">Add a Director to a Movie</a>
</p>
<p></p>
<h2 style="text-align:center"><font size = "3"><b>Browse Database</b></font></h2>
<p style="text-align:center">
<a href="./browse_actor.php">Actor</a>
</p>

<p style="text-align:center">
<a href="./browse_movie.php">Movie</a>
</p>




</body>
</html>