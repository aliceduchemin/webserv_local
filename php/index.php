<html>
<head>
    <meta charset="UTF-8">
    <title>A simple php page</title>
	<style>
		.container {
			color: #0c3285;
		}
		body {
			background-color: bisque;
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
        <h2>If you can see the date, php is working </h2>
        <h1><?php echo date('l jS \of F Y h:i:s A'); ?></h1>
        
        <h2>Args passed in query :</h2>
        <ul>
        <?php
            foreach ($_GET as $key => $value)
            {
                echo ("<li>" . $key . " : " . $value . "</li>");
            }

        ?>
        </ul>

        <a href="./index.php?did%20it%20worked=great&am%20i%20happy=yes">Try a get request with some args in url's query string</a>
    </div>
</body>
</html>