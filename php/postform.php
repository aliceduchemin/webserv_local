<html>
	<head>
		<title>Form to fill !</title>
		<style>
			body {
				background-color: #0c3285;
			}
			h1 {
				color: bisque;
				font-size: 55px;
			}
			#search2 {
				width: 15em;
				height: 2em;
				font-size: 20px;
			}
			.container {
				color: bisque;
				font-size: 45px;
				max-width: 1000px;
			}
			.gap-40 {
				width: 100%;
				height: 40px;
			}
			.gap-10 {
				width: 100%;
				height: 10px;
			}
		</style>
	</head>
	<body>
		<div class="gap-10"></div>
		<h1>Fill this form to keep in touch with us :</h1>
		<div class="gap-40"></div>
		<div class="container">
			<form action="welcome.php" method="post">
				Name: <input type="text" id="search2" name="name" placeholder="Enter your name here" />
				Email: <input type="text" id="search2" name="email" placeholder="Enter your email here" />
				<p><input type="submit" value="Send"></p>
			</form>
		</div>
	</body>
</html>