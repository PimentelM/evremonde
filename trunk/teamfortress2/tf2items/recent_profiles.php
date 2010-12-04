<!doctype html>

<html>

<head>

    <meta http-equiv="content-type" content="text/html; charset=utf-8" />

    <link rel="shortcut icon" href="http://www.teamfortress.com/war/part7/images/favicon.ico">

    <link rel="stylesheet" type="text/css" href="index.css" />

    <script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.4/jquery.min.js"></script>

    <title>Team Fortress 2 Recent Profiles</title>

</head>

<body>

    <div id="id_page_content">

        <center>

            <h1>Team Fortress 2 Recent Profiles</h1>

        </center>

        <div id="id_recent_profiles"></div>

    </div>

    <script type="text/javascript">

        <?php

            $data_recent_profiles = file_get_contents('http://www.tf2items.com/profiles/');

            $file_recent_profiles = 'recent_profiles.txt';

            file_put_contents('profiles/' . $file_recent_profiles, $data_recent_profiles);

        ?>

        $.ajax
        (
            {
                type: 'GET',
                url: 'profiles/recent_profiles.txt',
                dataType: 'html',
                success: function(html)
                {
                    $(html).find('.avatarIcon').each
                    (
                        function()
                        {
                            var link = $(this).find('a[href*="/profiles/7656"]').attr('href');

                            var avatar = $(this).find('img[src*="avatars"]').attr('src');

                            var name = $(this).find('img[src*="avatars"]').attr('alt');

                            var steamId64 = link.replace('/profiles/', '');

                            $('#id_recent_profiles').append('<div style="float: left; width: 250px; height: 50px">' + '<img style="margin: 10px; vertical-align: middle" src=' + avatar + ' />' + '<a href="index.php?profile=' + steamId64 + '">' + name + '</a>' + '<div />');
                        }
                    );
                }
            }
        );
        
    </script>

</body>

</html>
