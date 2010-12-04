<!doctype html>

<html>

<head>

    <meta http-equiv="content-type" content="text/html; charset=utf-8" />

    <link rel="shortcut icon" href="http://www.teamfortress.com/war/part7/images/favicon.ico">

    <link rel="stylesheet" type="text/css" href="index.css" />

    <script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.4/jquery.min.js"></script>

    <title>Team Fortress 2 All Items</title>

</head>

<body>

    <div id="id_page_content">

        <center>

            <h1>Team Fortress 2 All Items</h1>

        </center>

        <div id="id_items"></div>

    </div>

    <?php

        $tf2items_key = '6764751CE012B8295D6B1FD856158A0A';

        $language = $_GET['language'];

        if (!isset($language))
            $language = 'en';

        $data_GetSchema = file_get_contents('http://api.steampowered.com/ITFItems_440/GetSchema/v0001/?key=' . $tf2items_key . '&format=json' . '&language=' . $language);

        $file_GetSchema = 'tf_items_schema.txt';

        file_put_contents('GetSchema/' . $file_GetSchema, $data_GetSchema);
    ?>

    <script type="text/javascript">

        $.getJSON
        (
            'GetSchema/tf_items_schema.txt',
            function(GetSchema)
            {
                $.each
                (
                    GetSchema.result.items.item,
                    function(GetSchema_index, GetSchema_item)
                    {
                        var uniqueId = GetSchema_index;

                        var itemImage = GetSchema_item.image_url; //'images/' + GetSchema_item.image_inventory + '.png';

                        itemImage = itemImage.replace('\\', '');

                        if (GetSchema_item.item_type_name == 'CheatDetector')
                            itemImage = 'images/unknown.png';

                        $('#id_items').append('<div id="id_slot_' + uniqueId + '" class="class_slot"></div>');

                        $('<img />')
                            .attr('id',   'id_item_' + uniqueId)
                            .attr('class', 'class_item')
                            .attr('src',   itemImage)
                            .attr('title', GetSchema_item.name)
                            .attr('alt',   GetSchema_item.name)
                            .appendTo('#id_slot_' + uniqueId);
                    }
                );
            }
        );

    </script>

</body>

</html>
