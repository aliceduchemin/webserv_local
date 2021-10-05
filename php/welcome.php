<html>
	<head>
		<title>Welcome</title>
		<style>
			body {
				background-color: #0c3285;
			}
			.container {
				color: bisque;
				font-size: 45px;
				max-width: 1000px;
			}
			.gap-10 {
				width: 100%;
				height: 10px;
			}
		</style>
	</head>
	<body>
		<div class="gap-10"></div>
		<div class="container">
			Welcome <?php echo $_POST["name"]; ?><br>
			Your email address is <?php echo $_POST["email"]; ?><br>
		</div>
	</body>
</html>