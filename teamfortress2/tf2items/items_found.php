<html>

<head>

    <meta http-equiv="content-type" content="text/html; charset=utf-8" />

    <link rel="shortcut icon" href="http://www.teamfortress.com/war/part7/images/favicon.ico">

    <link rel="stylesheet" type="text/css" href="index.css" />

    <title>Team Fortress 2 Items Found</title>

</head>

<body>

    <?php

        $tf2items_key = '6764751CE012B8295D6B1FD856158A0A';

        $language = $_GET['language'];

        if (!isset($language))
            $language = 'en';

        $data_GetSchema = file_get_contents('http://api.steampowered.com/ITFItems_440/GetSchema/v0001/?key=' . $tf2items_key . '&format=json' . '&language=' . $language);
    
        $GetSchema = json_decode($data_GetSchema, false);

        $steamId64 = $_GET['id'];

        if (!isset($steamId64))
            $steamId64 = '76561197984116184';

        $data_GetPlayerSummaries = file_get_contents('http://api.steampowered.com/ISteamUser/GetPlayerSummaries/v0001/?key=' . $tf2items_key . '&steamids=' . $steamId64);

        $GetPlayerSummaries = json_decode($data_GetPlayerSummaries, false);

        foreach ($GetPlayerSummaries->response->players->player as $GetPlayerSummaries_player)
        {
            if ($GetPlayerSummaries_player->steamid == $steamId64)
            {
                echo $steamId64 . '<br />';

                echo $GetPlayerSummaries_player->personaname . '<br />';

                echo $GetPlayerSummaries_player->profileurl . '<br />';

                echo '<br />';

                if ($GetPlayerSummaries_player->personastate >= 1)
                {
                    echo 'Online<br />';

                    echo $GetPlayerSummaries_player->gameextrainfo . '<br />';
                    echo $GetPlayerSummaries_player->gameserverip . '<br />';
                }
                else
                {
                    echo 'Offline<br />';
                }
            }
        }

        $data_GetPlayerItems = file_get_contents('http://api.steampowered.com/ITFItems_440/GetPlayerItems/v0001/?key=' . $tf2items_key . '&SteamID=' . $steamId64);

        $GetPlayerItems = json_decode($data_GetPlayerItems, false);

        foreach ($GetSchema->result->items->item as $GetSchema_item)
        {
            foreach ($GetPlayerItems->result->items->item as $GetPlayerItems_item)
            {
                if ($GetPlayerItems_item->defindex == $GetSchema_item->defindex)
                {
                    $itemPosition = $GetPlayerItems_item->inventory & 0x0000FFFF;

                    if ($itemPosition == 0)
                    {
                        $itemName = $GetSchema_item->item_name;

                        if ($GetSchema_item->proper_name == true)
                            $itemName = $GetSchema_item->name;

                        echo '<img src="' . $GetSchema_item->image_url . '" alt="' . $itemName . '" title="' . $itemName . '" />';
                    }
                }
            }
        }

    ?>

</body>

</html>
