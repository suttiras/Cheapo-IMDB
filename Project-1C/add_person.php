<html>
<head>
	<title>IMDB: Cheapo Version</title>
</head>
<style>
.error {color: #FF0000;}
</style>
<body>
<p style="text-align:center"><a href="./Homepage.php"><img src="ImdbLogo.png" alt="Website Logo"></a></p>

<p style="text-align:center">
Actor/Director Page
</p>
<p>
<script type ="text/javascript">
	function checkForm(form)
	{
		var alphabet = /^[a-zA-Z ]*$/;
		if(!alphabet.test(form.first_name.value) || !alphabet.test(form.last_name.value)  )
		{
			alert("Error: Name(s) contain(s) invalid characters.");
			return false;
		}
		var date_valid = /^(?:(?:(?:(?:(?:[13579][26]|[2468][048])00)|(?:[0-9]{2}(?:(?:[13579][26])|(?:[2468][048]|0[48]))))(?:(?:(?:09|04|06|11)(?:0[1-9]|1[0-9]|2[0-9]|30))|(?:(?:01|03|05|07|08|10|12)(?:0[1-9]|1[0-9]|2[0-9]|3[01]))|(?:02(?:0[1-9]|1[0-9]|2[0-9]))))|(?:[0-9]{4}(?:(?:(?:09|04|06|11)(?:0[1-9]|1[0-9]|2[0-9]|30))|(?:(?:01|03|05|07|08|10|12)(?:0[1-9]|1[0-9]|2[0-9]|3[01]))|(?:02(?:[01][0-9]|2[0-8])))))$/;
		if(!date_valid.test(form.dob.value))
		{
			alert("Error: Date of birth is invalid.");
			return false;
		}
		if (form.dod.value != "" && !date_valid.test(form.dod.value))
		{
			alert("Error: Date of death is invalid.");
			return false;
		}
	}
</script>

<form method="GET" action="" onsubmit="return checkForm(this);"> 
   First Name: <input type="text" name="first_name" required>
   <span class="error">* <?php echo $firstnameErr;?></span>
   <br><br>
   
   Last Name: <input type="text" name="last_name" required>
	<span class="error">* <?php echo $lastnameErr;?></span>
   <br><br>
   Gender:
   <input checked= "checked" type="radio" name="gender" value='female'>Female
   <input type="radio" name="gender" value='male'>Male
   <span class="error">* <?php echo $genderErr;?></span>
   <br><br>
   Date of Birth: <input type="number" name="dob" required>
	<span class="error">* <?php echo $dobErr;?></span>
   <br><br>
   Date of Death (if applicable): <input type="number" name="dod">
   <br><br>

   <br><br>
   <input type="submit" name="submit" value="Submit">
   
   <p>
	<?php
		//echo "Hello World!";
		
	?>
</p>
   
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