<!doctype html>

<html>

<head>

    <meta http-equiv="content-type" content="text/html; charset=utf-8" />

    <link rel="shortcut icon" href="http://www.teamfortress.com/war/part7/images/favicon.ico">

    <link rel="stylesheet" type="text/css" href="index.css" />

    <script type="text/javascript" src="http://ajax.googleapis.com/ajax/libs/jquery/1.4.4/jquery.min.js"></script>

    <title>Team Fortress 2 Backpack Viewer</title>

</head>

<body>

    <div id="id_page_content">

        <center>

            <h1 id="id_page_header">Team Fortress 2 Backpack Viewer</h1>

            <div id="id_profile_submit_content">

                <div id="id_profile_submit">

                    <form action="index.php" method"get">

                        <table>
                            <tr>
                                <td><b>Profile:</b></td>
                                <td>
                                    <input type="text" id="id_profile_submit_text" name="profile" />
                                </td>
                                <td>
                                    <input type="submit" id="id_profile_submit_button" value="Submit" />
                                </td>
                            </tr>
                        </table>

                    <table>
                        <tr>
                            <td>
                                <input type="checkbox" id="id_profile_submit_checkbox_show_all_pages" name="show_all_pages" checked>Show all pages</input>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <input type="checkbox" id="id_profile_submit_checkbox_show_equipped" name="show_equipped" checked>Show equipped</input>
                            </td>
                            <td>
                                <input type="checkbox" id="id_profile_submit_checkbox_highlight_hats" name="highlight_hats" checked>Highlight hats</input>
                            </td>
                        </tr>
                        <tr>
                            <td>
                                <input type="checkbox" id="id_profile_submit_checkbox_show_levels" name="show_levels" checked>Show levels</input>
                            </td>
                            <td>
                                <input type="checkbox" id="id_profile_submit_checkbox_quality_borders" name="quality_borders" checked>Quality borders</input>
                            </td>
                        </tr>
                    </table>

                    </form>

                </div>

            </div>

        </center>

        <div style="clear: both"> </div>

        <div id="id_profile_content">

            <h2 id="id_profile_header">Profile</h2>

            <div id="id_profile_summary">

                <h3 id="id_profile_summary_steam_id"></h3>

                <div id="id_profile_summary_avatar">

                    <div id="id_valve_logo_content">

                        <img id="id_valve_logo" class="class_valve_logo" src="images/valve.png" />
                        <br />

                    </div>

                    <img id="id_avatar" class="class_avatar" src="images/unknown.png" />

                </div>

                <p>

                <div id="id_profile_summary_profile_error"></div>

            </div>

        </div>

        <div style="clear: both"> </div>

        <div id="id_items_preview_content">

            <h2 id="id_items_preview_header">Backpack Preview</h2>

            <div id="id_items_preview"></div>

            <div id="id_items_preview_text"></div>

        </div>

        <div style="clear: both"> </div>

        <div id="id_items_content">

            <h2 id="id_items_header">Backpack</h2>

            <div id="id_items"></div>

        </div>

        <div style="clear: both"> </div>

        <div id="id_items_found_content">

            <h2 id="id_items_found_header">Items Found</h2>

            <p id="id_items_found_description">The following items were found but not yet placed in the backpack:</p>

            <div id="id_items_found"></div>

        </div>

        <div style="clear: both"> </div>

        <div id="id_summary_content">

            <h2 id="id_summary_header">Summary</h2>

            <p id="id_summary_warning"></p>

            <div id="id_summary">

                <table>
                    <tr>
                        <td><b>Items:</b></td>
                        <td id="id_summary_n_items"></td>
                    </tr>
                    <tr>
                        <td><b>Found:</b></td>
                        <td id="id_summary_n_items_found"></td>
                    </tr>
                    <tr>
                        <td></td>
                        <td></td>
                    </tr>
                    <tr>
                        <td><b>Unusuals:</b></td>
                        <td id="id_summary_n_unusuals"></td>
                    </tr>
                    <tr>
                        <td><b>Hats:</b></td>
                        <td id="id_summary_n_hats"></td>
                    </tr>
                    <tr>
                        <td><b>Metals:</b></td>
                        <td id="id_summary_n_metals"></td>
                    </tr>
                    <tr>
                        <td><b>Crates:</b></td>
                        <td id="id_summary_n_crates"></td>
                    </tr>
                    <tr>
                        <td><b>Keys:</b></td>
                        <td id="id_summary_n_keys"></td>
                    </tr>
                    <tr>
                        <td><b>Tags</b></td>
                        <td id="id_summary_n_tags"></td>
                    </tr>
                    <tr>
                        <td><b>Paints:</b></td>
                        <td id="id_summary_n_paints"></td>
                    </tr>
                    <tr>
                        <td><b>Gifts:</b></td>
                        <td id="id_summary_n_gifts"></td>
                    </tr>
                    <tr>
                        <td><b>Tokens:</b></td>
                        <td id="id_summary_n_tokens"></td>
                    </tr>
                    <tr>
                        <td></td>
                        <td></td>
                    </tr>
                    <tr>
                        <td><b>Total:</b></td>
                        <td id="id_summary_n_items_total"></td>
                    </tr>
                </table>

            </div>

        </div>

        <br />

    </div>

    <?php
       $mtime = microtime();
       $mtime = explode(" ", $mtime);
       $mtime = $mtime[1] + $mtime[0];
       $starttime = $mtime;
    ?>

    <?php

        $tf2items_key = '6764751CE012B8295D6B1FD856158A0A';

        $language = $_GET['language'];

        if (!isset($language))
            $language = 'en';

        $data_GetSchema = file_get_contents('http://api.steampowered.com/ITFItems_440/GetSchema/v0001/?key=' . $tf2items_key . '&format=json' . '&language=' . $language);

        $file_GetSchema = 'tf_items_schema.txt';

        file_put_contents('GetSchema/' . $file_GetSchema, $data_GetSchema);

        function is_steamId64($value)
        { 
            if (is_numeric($value) && strlen($value) == 17)
                return true;
            else
                return false;
        }

        $profile = $_GET['profile'];

        if (strlen($profile) > 0)
        {
            if (is_steamId64($profile))
                $profileType = 'profiles';
            else
                $profileType = 'id';

            $data_xml_profile = file_get_contents('http://steamcommunity.com/'. $profileType . '/' . $profile . '?xml=1');

            $file_xml_profile = $profile . '.xml';

            file_put_contents('profiles/' . $file_xml_profile, $data_xml_profile);

            $parse_xml_profile = simplexml_load_file('profiles/' . $file_xml_profile);

            if (is_steamId64($profile))
            {
                $steamId64 = $profile;
            }
            else
            {
                foreach($parse_xml_profile->children() as $parse_xml_profile_child)
                {
                    if ($parse_xml_profile_child->getName() == 'steamID64')
                        $steamId64 = $parse_xml_profile_child;
                }
            }

            $data_GetPlayerSummaries = file_get_contents('http://api.steampowered.com/ISteamUser/GetPlayerSummaries/v0001/?key=' . $tf2items_key . '&steamids=' . $steamId64);

            if (is_steamId64($profile))
                $file_GetPlayerSummaries = $steamId64 . '.txt';
            else
                $file_GetPlayerSummaries = $profile . '.txt';

            file_put_contents('GetPlayerSummaries/' . $file_GetPlayerSummaries, $data_GetPlayerSummaries);

            $data_GetPlayerItems = file_get_contents('http://api.steampowered.com/ITFItems_440/GetPlayerItems/v0001/?key=' . $tf2items_key . '&SteamID=' . $steamId64);

            if (is_steamId64($profile))
                $file_GetPlayerItems = $steamId64 . '.txt';
            else
                $file_GetPlayerItems = $profile . '.txt';

            file_put_contents('GetPlayerItems/' . $file_GetPlayerItems, $data_GetPlayerItems);
        }
    ?>

    <script type="text/javascript">

        $.extend
        (
            {
                getUrlVars: function()
                {
                    var vars = [], hash;
                    var hashes = window.location.href.slice(window.location.href.indexOf('?') + 1).split('&');
                    for (var i = 0; i < hashes.length; i++)
                    {
                        hash = hashes[i].split('=');
                        vars.push(hash[0]);
                        vars[hash[0]] = hash[1];
                    }
                    return vars;
                },
                getUrlVar: function(name)
                {
                    return $.getUrlVars()[name];
                }
            }
        );

        function isNumeric(n) { return !isNaN(parseFloat(n)) && isFinite(n); }

        var queryProfile = $.getUrlVar('profile');

        if (!queryProfile)
        {
            $('#id_profile_content').hide();
            $('#id_items_content').hide();
            $('#id_items_found_content').hide();
            $('#id_summary_content').hide();
        }

        $('#id_profile_submit_text').attr('value', queryProfile);

        var queryShowAllPages = $.getUrlVar('show_all_pages');

        if (queryShowAllPages == 'on')
            $('#id_profile_submit_checkbox_show_all_pages').attr('checked', true);
        else
            $('#id_profile_submit_checkbox_show_all_pages').attr('checked', false);

        var queryShowEquipped = $.getUrlVar('show_equipped');

        if (queryShowEquipped == 'on')
            $('#id_profile_submit_checkbox_show_equipped').attr('checked', true);
        else
            $('#id_profile_submit_checkbox_show_equipped').attr('checked', false);

        var queryShowLevels = $.getUrlVar('show_levels');

        if (queryShowLevels == 'on')
            $('#id_profile_submit_checkbox_show_levels').attr('checked', true);
        else
            $('#id_profile_submit_checkbox_show_levels').attr('checked', false);

        var queryHighlightHats = $.getUrlVar('highlight_hats');

        if (queryHighlightHats == 'on')
            $('#id_profile_submit_checkbox_highlight_hats').attr('checked', true);
        else
            $('#id_profile_submit_checkbox_highlight_hats').attr('checked', false);

        var queryQualityBorders = $.getUrlVar('quality_borders');

        if (queryQualityBorders == 'on')
            $('#id_profile_submit_checkbox_quality_borders').attr('checked', true);
        else
            $('#id_profile_submit_checkbox_quality_borders').attr('checked', false);

        $('#id_valve_logo_content').hide();

        $.ajax
        (
            {
                type: 'GET',
                url: 'profiles/' + queryProfile + '.xml',
                dataType: 'xml',
                success: function(xml)
                {
                    var responseError = $(xml).find('error:first').text();

                    if (responseError.length > 0 && responseError == 'The specified profile could not be found.')
                    {
                        $('#id_profile_summary_profile_error')
                            .attr('class', 'class_profile_summary_profile_not_found')
                            .html('<p><b>Error!</b> The specified profile could not be found.</p>')
                            .show();
                    }

                    var groupId64 = $(xml).find('groupID64:first').text();

                    if (groupId64 == "103582791429521412")
                        $('#id_valve_logo_content').show();
                }
            }
        );

        $.getJSON
        (
            'GetPlayerSummaries/' + queryProfile + '.txt',
            function(GetPlayerSummaries)
            {
                $.each
                (
                    GetPlayerSummaries.response.players.player,
                    function(GetPlayerSummaries_index, GetPlayerSummaries_player)
                    {
                        var communityVisibilityState = GetPlayerSummaries_player.communityvisibilitystate;

                        if (communityVisibilityState != 3)
                        {
                            var privacyText = '';

                            privacyText += '<p>';

                            privacyText += '<b>Sorry!</b> The specified profile is private.';

                            if (communityVisibilityState == 2)
                                 privacyText += ' (Friends Only)';

                            privacyText += '</p>';

                            $('#id_profile_summary_profile_error')
                                .attr('class', 'class_profile_summary_profile_is_private')
                                .html(privacyText)
                                .show();
                        }

                        if ($('#id_profile_summary_profile_error').is(':visible'))
                        {
                            $('#id_items_content').hide();
                            $('#id_items_found_content').hide();
                            $('#id_summary_content').hide();
                        }

                        $('#id_profile_summary_steam_id').text(GetPlayerSummaries_player.personaname);

                        var avatarImage = GetPlayerSummaries_player.avatarfull;

                        avatarImage = avatarImage.replace('\\', '');

                        $('#id_avatar').attr('src', avatarImage);
                    }
                );
            }
        );

        $('#id_items_preview').append('<div id="id_items_preview_page_1"></div>');
        $('#id_items_preview').append('<div id="id_items_preview_page_2"></div>');
        $('#id_items_preview').append('<div id="id_items_preview_page_3"></div>');
        $('#id_items_preview').append('<div id="id_items_preview_page_4"></div>');

        $('#id_items').append('<div id="id_items_page_1"></div>');
        $('#id_items').append('<div style="clear: both"> </div>');
        $('#id_items').append('<div id="id_items_page_2"></div>');
        $('#id_items').append('<div style="clear: both"> </div>');
        $('#id_items').append('<div id="id_items_page_3"></div>');
        $('#id_items').append('<div style="clear: both"> </div>');
        $('#id_items').append('<div id="id_items_page_4"></div>');
        $('#id_items').append('<div style="clear: both"> </div>');
        
        if (queryShowAllPages == 'on')
        {
            $('#id_items_page_2').css('margin-top', '48px');
            $('#id_items_page_3').css('margin-top', '48px');
            $('#id_items_page_4').css('margin-top', '48px');
        }
        else
        {
            $('#id_items_page_2').hide();
            $('#id_items_page_3').hide();
            $('#id_items_page_4').hide();

            $('#id_items').append('<div id="id_items_buttons"></div>');

            $('#id_items_buttons').append('<div id="id_items_buttons_content"></div>');

            $('<img />')
                .attr('id', 'id_items_button_prev')
                .attr('src', 'images/buttons/prev.png')
                .css('cursor', 'pointer')
                .appendTo('#id_items_buttons_content');

            $('<img />')
                .attr('id', 'id_items_page_number')
                .attr('src', 'images/pages/page_1_of_4.png')
                .css('cursor', 'default')
                .appendTo('#id_items_buttons_content');

            $('<img />')
                .attr('id', 'id_items_button_next')
                .attr('src', 'images/buttons/next.png')
                .css('cursor', 'pointer')
                .appendTo('#id_items_buttons_content');

            $('#id_items_button_prev').click
            (
                function()
                {
                    if ($('#id_items_page_number').attr('src') == 'images/pages/page_1_of_4.png')
                    {
                        $('#id_items_page_number').attr('src', 'images/pages/page_4_of_4.png');

                        $('#id_items_page_1').hide();
                        $('#id_items_page_2').hide();
                        $('#id_items_page_3').hide();
                        $('#id_items_page_4').show();
                    }
                    else if ($('#id_items_page_number').attr('src') == 'images/pages/page_2_of_4.png')
                    {
                        $('#id_items_page_number').attr('src', 'images/pages/page_1_of_4.png');

                        $('#id_items_page_1').show();
                        $('#id_items_page_2').hide();
                        $('#id_items_page_3').hide();
                        $('#id_items_page_4').hide();
                    }
                    else if ($('#id_items_page_number').attr('src') == 'images/pages/page_3_of_4.png')
                    {
                        $('#id_items_page_number').attr('src', 'images/pages/page_2_of_4.png');

                        $('#id_items_page_1').hide();
                        $('#id_items_page_2').show();
                        $('#id_items_page_3').hide();
                        $('#id_items_page_4').hide();
                    }
                    else if ($('#id_items_page_number').attr('src') == 'images/pages/page_4_of_4.png')
                    {
                        $('#id_items_page_number').attr('src', 'images/pages/page_3_of_4.png');

                        $('#id_items_page_1').hide();
                        $('#id_items_page_2').hide();
                        $('#id_items_page_3').show();
                        $('#id_items_page_4').hide();
                    }
                }
            );

            $('#id_items_button_next').click
            (
                function()
                {
                    if ($('#id_items_page_number').attr('src') == 'images/pages/page_1_of_4.png')
                    {
                        $('#id_items_page_number').attr('src', 'images/pages/page_2_of_4.png');

                        $('#id_items_page_1').hide();
                        $('#id_items_page_2').show();
                        $('#id_items_page_3').hide();
                        $('#id_items_page_4').hide();
                    }
                    else if ($('#id_items_page_number').attr('src') == 'images/pages/page_2_of_4.png')
                    {
                        $('#id_items_page_number').attr('src', 'images/pages/page_3_of_4.png');

                        $('#id_items_page_1').hide();
                        $('#id_items_page_2').hide();
                        $('#id_items_page_3').show();
                        $('#id_items_page_4').hide();
                    }
                    else if ($('#id_items_page_number').attr('src') == 'images/pages/page_3_of_4.png')
                    {
                        $('#id_items_page_number').attr('src', 'images/pages/page_4_of_4.png');

                        $('#id_items_page_1').hide();
                        $('#id_items_page_2').hide();
                        $('#id_items_page_3').hide();
                        $('#id_items_page_4').show();
                    }
                    else if ($('#id_items_page_number').attr('src') == 'images/pages/page_4_of_4.png')
                    {
                        $('#id_items_page_number').attr('src', 'images/pages/page_1_of_4.png');

                        $('#id_items_page_1').show();
                        $('#id_items_page_2').hide();
                        $('#id_items_page_3').hide();
                        $('#id_items_page_4').hide();
                    }
                }
            );

            $('#id_items_button_prev').hover
            (
                function()
                {
                    $('#id_items_button_prev').attr('src', 'images/buttons/prev_hover.png');
                },
                function()
                {
                    $('#id_items_button_prev').attr('src', 'images/buttons/prev.png');
                }
            );

            $('#id_items_button_next').hover
            (
                function()
                {
                    $('#id_items_button_next').attr('src', 'images/buttons/next_hover.png');
                },
                function()
                {
                    $('#id_items_button_next').attr('src', 'images/buttons/next.png');
                }
            );
        }

        for (var i = 1; i < 201; i++)
        {
            var backpackPage = Math.floor(i / 50);

            if (i % 50 > 0)
                backpackPage += 1;

            $('#id_items_preview_page_' + backpackPage).append('<div id="id_preview_slot_' + i + '" class="class_preview_slot"></div>');

            $('#id_preview_slot_' + i).append('<div id="id_preview_item_' + i + '" class="class_preview_item"></div>');

            $('#id_preview_item_' + i)
                .css('cursor', 'pointer')
                .css('width',  '8px')
                .css('height', '8px')
                .css('background-color', '#000000');

            $('#id_items_page_' + backpackPage).append('<div id="id_slot_' + i + '" class="class_slot"></div>');

            $('<img />')
                .attr('id',    'id_item_' + i)
                .attr('class', 'class_item')
                .attr('src',   'images/blank.png')
                .appendTo('#id_slot_' + i);
        }

        var nItems      = 0;
        var nItemsFound = 0;
        var nUnusuals   = 0;
        var nHats       = 0;
        var nMetals     = 0;
        var nCrates     = 0;
        var nKeys       = 0;
        var nTags       = 0;
        var nPaints     = 0;
        var nGifts      = 0;
        var nTokens     = 0;

        var nScrapMetals     = 0;
        var nReclaimedMetals = 0;
        var nRefinedMetals   = 0;

        $.getJSON
        (
            'GetSchema/tf_items_schema.txt',
            function(GetSchema)
            {
                $.each
                (
                    GetSchema.result.items.item,
                    function(GetSchema_item_index, GetSchema_item)
                    {
                        $.getJSON
                        (
                            'GetPlayerItems/' + queryProfile + '.txt',
                            function(GetPlayerItems)
                            {
                                $.each
                                (
                                    GetPlayerItems.result.items.item,
                                    function(GetPlayerItems_item_index, GetPlayerItems_item)
                                    {
                                        if (GetPlayerItems_item.defindex == GetSchema_item.defindex)
                                        {
                                            var itemPosition   = GetPlayerItems_item.inventory & 0x0000FFFF;

                                            var itemIsEquipped = GetPlayerItems_item.inventory & 0x0FFF0000;

                                            var itemName = GetSchema_item.item_name;

                                            if (GetSchema_item.proper_name == true)
                                            {
                                                itemName = GetSchema_item.name;

                                                // sam and max items proper_name hack
                                                switch (GetSchema_item.defindex)
                                                {
                                                    case 160: // Lugermorph
                                                    case 161: // The Big Kill

                                                    itemName = GetSchema_item.item_name;
                                                }
                                            }

                                            var itemImage = GetSchema_item.image_url; //'images/' + GetSchema_item.image_inventory + '.png';

                                            itemImage = itemImage.replace('\\', '');

                                            var itemQualityPrefix = '';
                                            var itemQualityColor  = '#FFFF33'; // default

                                            switch (GetPlayerItems_item.quality)
                                            {
                                                case 3:
                                                    itemQualityPrefix = 'Vintage';
                                                    itemQualityColor  = '#0033FF';
                                                    break;
                                                case 5:
                                                    itemQualityPrefix = 'Unusual';
                                                    itemQualityColor  = '#6633FF';
                                                    break;
                                                case 7:
                                                    itemQualityPrefix = 'Community';
                                                    itemQualityColor  = '#33FF33';
                                                    break;
                                                case 8:
                                                    itemQualityPrefix = 'Valve';
                                                    itemQualityColor  = '#FF33FF';
                                                    break;
                                                case 9:
                                                    itemQualityPrefix = 'Self-Made';
                                                    itemQualityColor  = '#00FF66';
                                                    break;
                                                case 10:
                                                    itemQualityPrefix = 'Customized';
                                                    itemQualityColor  = '#FF3300';
                                                    break;
                                            }

                                            if (itemPosition === 0)
                                            {
                                                nItemsFound++;
                                            }
                                            else
                                            {
                                                nItems++;
                                            }

                                            if (GetPlayerItems_item.quality == 5)
                                                nUnusuals++;

                                            if
                                            (
                                                GetSchema_item.craft_class    == 'hat'              ||
                                                GetSchema_item.item_type_name == 'TF_Wearable_Hat'  ||
                                                GetSchema_item.item_type_name == '#TF_Wearable_Hat' ||
                                                GetSchema_item.item_slot      == 'head'
                                            )
                                            {
                                                nHats++;
                                            }

                                            if (GetSchema_item.craft_class == 'craft_bar')
                                            {
                                                nMetals++;

                                                if (GetSchema_item.name == 'Craft Bar Level 1')
                                                    nScrapMetals++;
                                                else if (GetSchema_item.name == 'Craft Bar Level 2')
                                                    nReclaimedMetals++;
                                                else if (GetSchema_item.name == 'Craft Bar Level 3')
                                                    nRefinedMetals++;
                                            }

                                            if (GetSchema_item.item_type_name == 'TF_LockedCrate')
                                                nCrates++;

                                            if (GetSchema_item.defindex == 5021)
                                                nKeys++;

                                            if (GetSchema_item.craft_class == 'tool')
                                            {
                                                if (GetSchema_item.name.indexOf('Paint') != -1)
                                                    nPaints++;

                                                if (GetSchema_item.name.indexOf('Tag') != -1)
                                                    nTags++;
                                            }

                                            if (GetSchema_item.item_type_name == 'Gift')
                                                nGifts++;

                                            if (GetSchema_item.craft_class == 'craft_token')
                                                nTokens++;

                                            var positionType = '#id_items';

                                            if (itemPosition === 0)
                                                positionType = '#id_items_found';

                                            var uniqueId = -1;

                                            if (itemPosition > 0)
                                                uniqueId = itemPosition;
                                            else
                                                uniqueId = uniqueId - GetPlayerItems_item_index;

                                            if (itemPosition > 0)
                                            {
                                                $('#id_preview_item_' + uniqueId)
                                                    .css('background-color', itemQualityColor)
                                                    .attr('title', itemName)
                                                    .attr('alt',   itemName);

                                                $('#id_preview_item_' + uniqueId).hover
                                                (
                                                    function()
                                                    {
                                                        $('#id_items_preview_text')
                                                            .css('color', itemQualityColor)
                                                            .text(itemQualityPrefix + ' ' + itemName);
                                                    },
                                                    function()
                                                    {
                                                        $('#id_items_preview_text').text('');
                                                    }
                                                );

                                                $('#id_item_' + uniqueId)
                                                    .attr('src',   itemImage)
                                                    .attr('title', itemName)
                                                    .attr('alt',   itemName);
                                            }
                                            else
                                            {
                                                $(positionType).append('<div id="id_slot_' + uniqueId + '" class="class_slot"></div>');

                                                $('<img />')
                                                    .attr('id',    'id_item_' + uniqueId)
                                                    .attr('class', 'class_item')
                                                    .attr('src',   itemImage)
                                                    .attr('title', itemName)
                                                    .attr('alt',   itemName)
                                                    .appendTo('#id_slot_' + uniqueId);
                                            }

                                            if (queryShowEquipped == 'on')
                                            {
                                                if (itemIsEquipped !== 0)
                                                    $('<div class="class_item_equipped">Equipped<div />').appendTo('#id_slot_' + uniqueId);
                                            }

                                            if (queryShowLevels == 'on')
                                                $('<div id= "id_item_level_' + uniqueId + '" class="class_item_level">Level ' + GetPlayerItems_item.level + '<div />').appendTo('#id_slot_' + uniqueId);

                                            if (queryHighlightHats == 'on')
                                            {
                                                if
                                                (
                                                    GetSchema_item.craft_class    == 'hat'              ||
                                                    GetSchema_item.item_type_name == 'TF_Wearable_Hat'  ||
                                                    GetSchema_item.item_type_name == '#TF_Wearable_Hat' ||
                                                    GetSchema_item.item_slot      == 'head'
                                                )
                                                {
                                                    $('#id_item_' + uniqueId).css('background-color', '#C8BBA2');
                                                }
                                            }

                                            if (queryQualityBorders == 'on')
                                                $('#id_item_' + uniqueId).css('border', '1px solid ' + itemQualityColor);

                                            if (itemPosition === 0)
                                                $('#id_item_' + uniqueId).css('background-color', '#2A2725');

                                            $('#id_item_' + uniqueId).attr('title', '');
                                            $('#id_item_' + uniqueId).attr('alt',   '');

                                            var tooltipItemName = itemName;

                                            if
                                            (
                                                 GetPlayerItems_item.quality == 3 ||
                                                 GetPlayerItems_item.quality == 5 ||
                                                (GetPlayerItems_item.quality >= 7 && GetPlayerItems_item.quality <= 9)
                                            )
                                            {
                                                tooltipItemName = itemQualityPrefix + ' ' + itemName;
                                            }

                                            var tooltipText = '';

                                            tooltipText += '<div id="id_tooltip_' + uniqueId + '" class="class_tooltip">';

                                            tooltipText += '<p>';

                                            tooltipText += '<font id="id_tooltip_item_name" color="' + itemQualityColor + '">' + tooltipItemName + '</font>';

                                            if (GetPlayerItems_item.custom_name)
                                            {
                                                tooltipText += '<br />';

                                                tooltipText += '<font class="class_tooltip_item_custom_name">' + '"' + GetPlayerItems_item.custom_name + '"' + '</font>';
                                            }

                                            if (GetPlayerItems_item.custom_desc)
                                            {
                                                tooltipText += '<br />';

                                                tooltipText += '<font class="class_tooltip_item_custom_desc">' + '"' + GetPlayerItems_item.custom_desc + '"' + '</font>';
                                            }

                                            if (GetPlayerItems_item.custom_name || GetPlayerItems_item.custom_desc)
                                            {
                                                $('<div id="id_item_tagged_' + uniqueId + '" class="class_item_tagged"><div />').appendTo('#id_slot_' + uniqueId);

                                                $('<img />')
                                                    .attr('width',  '32')
                                                    .attr('height', '32')
                                                    .attr('src', 'images/tagged.png')
                                                    .appendTo('#id_item_tagged_' + uniqueId);
                                            }

                                            if (itemIsEquipped !== 0)
                                            {
                                                tooltipText += '<br />';

                                                tooltipText += '<font class="class_tooltip_item_equipped">' + '(equipped)' + '</font>';
                                            }

                                            var nDuplicates = 0;

                                            $.each
                                            (
                                                GetPlayerItems.result.items.item,
                                                function(GetPlayerItems_duplicate_item_index, GetPlayerItems_duplicate_item)
                                                {
                                                    if (GetPlayerItems_duplicate_item.defindex == GetPlayerItems_item.defindex)
                                                        nDuplicates++;
                                                }
                                            );

                                            if (nDuplicates > 1)
                                            {
                                                tooltipText += '<br />';

                                                tooltipText += '<font class="class_tooltip_item_duplicate">' + '(' + nDuplicates + 'x' + ')' + '</font>';
                                            }

                                            tooltipText += '</p>';

                                            tooltipText += '<p class="class_tooltip_item_level">' + 'Level ' + GetPlayerItems_item.level;

                                            if (GetSchema_item.item_type_name != 'TF_Wearable_Hat')
                                            {
                                                tooltipText += ' ';

                                                if (GetSchema_item.item_type_name == 'TF_LockedCrate')
                                                    tooltipText += GetSchema_item.name;
                                                else
                                                    tooltipText += GetSchema_item.item_type_name;
                                            }

                                            //if (tf2items_item.holiday)
                                            //{
                                                //tooltipText += '<p class="class_tooltip_item_holiday">' + 'Holiday Restriction: ' + tf2items_item.holiday + '</p>';
                                            //}

                                            tooltipText += '</p>';

                                            if (GetPlayerItems_item.attributes)
                                            {
                                                tooltipText += '<p class="class_tooltip_item_attributes">';

                                                //tooltipText += 'Special Attributes' + '<br />';

                                                $.each
                                                (
                                                    GetPlayerItems_item.attributes.attribute,
                                                    function(GetPlayerItems_item_attr_index, GetPlayerItems_item_attr)
                                                    {
                                                        if (GetPlayerItems_item_attr.defindex == 132)
                                                            tooltipText += 'Given to valuable Community Contributors<br />';
                                                        else if (GetPlayerItems_item_attr.defindex == 133)
                                                            tooltipText += 'Medal no. ' + GetPlayerItems_item_attr.value + '<br />';
                                                        else if (GetPlayerItems_item_attr.defindex == 134)
                                                        {
                                                            $('<img />')
                                                                .attr('id',    'id_effect_' + uniqueId)
                                                                .attr('class', 'class_effect')
                                                                .attr('src',   'images/effects/' + GetPlayerItems_item_attr.float_value + '.png')
                                                                .appendTo('#id_slot_' + uniqueId);

                                                            tooltipText += 'Effect: ';

                                                            switch (GetPlayerItems_item_attr.float_value)
                                                            {
                                                                case 4:
                                                                    tooltipText += 'Community Sparkle';
                                                                    break;
                                                                case 7:
                                                                    tooltipText += 'Purple Confetti';
                                                                    break;
                                                                case 8:
                                                                    tooltipText += 'Haunted Ghosts';
                                                                    break;
                                                                case 9:
                                                                    tooltipText += 'Green Energy';
                                                                    break;
                                                                case 10:
                                                                    tooltipText += 'Purple Energy';
                                                                    break;
                                                                case 11:
                                                                    tooltipText += 'Circling TF Logo';
                                                                    break;
                                                                case 14:
                                                                    tooltipText += 'Scorching Flames';
                                                                    break;
                                                                case 15:
                                                                    tooltipText += 'Searing Plasma';
                                                                    break;
                                                                case 18:
                                                                    tooltipText += 'Circling Peace Sign';
                                                                    break;
                                                                default:
                                                                    tooltipText += GetPlayerItems_item_attr.float_value;
                                                            }

                                                            tooltipText += '<br />';
                                                        }
                                                        else if (GetPlayerItems_item_attr.defindex == 141)
                                                            tooltipText += 'I made this!<br />';
                                                        else if (GetPlayerItems_item_attr.defindex == 142)
                                                        {
                                                            $('<div id="id_item_painted_' + uniqueId + '" class="class_item_painted"><div />').appendTo('#id_slot_' + uniqueId);

                                                            $('<img />')
                                                                .attr('width', '32')
                                                                .attr('height', '32')
                                                                .attr('src', 'http://tf2b.com/imgmisc/paintsplat_' + GetPlayerItems_item_attr.float_value + '.png')
                                                                .appendTo('#id_item_painted_' + uniqueId);

                                                            tooltipText += 'Paint: ';

                                                            switch (GetPlayerItems_item_attr.float_value)
                                                            {
                                                                case 0:
                                                                    tooltipText += "None";
                                                                    break;
                                                                case 1:
                                                                    tooltipText += "Team Spirit";
                                                                    break;
                                                                case 7511618:
                                                                    tooltipText += "Indubitably Green";
                                                                    break;
                                                                case 4345659:
                                                                    tooltipText += "Zephaniah's Greed";
                                                                    break;
                                                                case 5322826:
                                                                    tooltipText += "Noble Hatter's Violet";
                                                                    break;
                                                                case 14204632:
                                                                    tooltipText += "Color No. 216-190-216";
                                                                    break;
                                                                case 8208497:
                                                                    tooltipText += "A Deep Commitment to Purple";
                                                                    break;
                                                                case 13595446:
                                                                    tooltipText += "Mann Co. Orange";
                                                                    break;
                                                                case 10843461:
                                                                    tooltipText += "Muskelmannbraun";
                                                                    break;
                                                                case 12955537:
                                                                    tooltipText += "Peculiarly Drab Tincture";
                                                                    break;
                                                                case 6901050:
                                                                    tooltipText += "Radigan Conagher Brown";
                                                                    break;
                                                                case 8154199:
                                                                    tooltipText += "Ye Olde Rustic Colour";
                                                                    break;
                                                                case 15185211:
                                                                    tooltipText += "Australium Gold";
                                                                    break;
                                                                case 8289918:
                                                                    tooltipText += "Aged Moustache Grey";
                                                                    break;
                                                                case 15132390:
                                                                    tooltipText += "An Extraordinary Abundance of Tinge";
                                                                    break;
                                                                case 1315860:
                                                                    tooltipText += "A Distinctive Lack of Hue";
                                                                    break;
                                                                default:
                                                                    tooltipText += GetPlayerItems_item_attr.float_value;
                                                            }

                                                            tooltipText += '<br />';
                                                        }
                                                        else if (GetPlayerItems_item_attr.defindex == 143)
                                                        {
                                                            var hireDate = new Date;
                                                            hireDate.setTime(GetPlayerItems_item_attr.value * 1000);

                                                            tooltipText += 'Hire Date: ' + hireDate.toLocaleDateString() + '<br />';
                                                        }
                                                        else if (GetPlayerItems_item_attr.defindex == 153)
                                                            tooltipText += 'Not Tradable<br />';
                                                        else if (GetPlayerItems_item_attr.defindex == 172)
                                                            tooltipText += 'Purchased: Not Tradable or Usable in Crafting<br />';
                                                        else if (GetPlayerItems_item_attr.defindex == 185)
                                                        {
                                                            $('<div id="id_item_gifted_' + uniqueId + '" class="class_item_gifted"><div />').appendTo('#id_slot_' + uniqueId);

                                                            $('<img />')
                                                                .attr('width', '32')
                                                                .attr('height', '32')
                                                                .attr('src', 'images/gifted.png')
                                                                .appendTo('#id_item_gifted_' + uniqueId);

                                                            var eventDate = new Date;
                                                            eventDate.setTime(GetPlayerItems_item_attr.value * 1000);

                                                            tooltipText += 'Gift<br />Date Received: ' + eventDate.toLocaleDateString() + '<br />';
                                                        }
                                                        else if (GetPlayerItems_item_attr.defindex == 187)
                                                        {
                                                            tooltipText += 'Crate Series #' + GetPlayerItems_item_attr.float_value + '<br />';

                                                            if (queryShowLevels == 'on')
                                                                $('#id_item_level_' + uniqueId).text('Series ' + GetPlayerItems_item_attr.float_value);
                                                        }
                                                    }
                                                )

                                                tooltipText += '</p>';
                                            }

                                            if (GetPlayerItems_item.quality >= 7 && GetPlayerItems_item.quality <= 9)
                                            {
                                                tooltipText += '<p class="class_tooltip_item_attributes">';

                                                tooltipText += 'Not Tradable or Usable in Crafting';

                                                tooltipText += '</p>';
                                            }
                                            else
                                            {
                                                if (GetPlayerItems_item.flag_cannot_trade == true)
                                                {
                                                    tooltipText += '<p class="class_tooltip_item_attributes">';

                                                    tooltipText += 'Not Tradable';

                                                    tooltipText += '</p>';
                                                }
                                                else
                                                {
                                                    if (GetSchema_item.attributes)
                                                    {
                                                        tooltipText += '<p class="class_tooltip_item_attributes">';

                                                        $.each
                                                        (
                                                            GetSchema_item.attributes.attribute,
                                                            function(GetSchema_item_attr_index, GetSchema_item_attr)
                                                            {
                                                                if (GetSchema_item_attr.class == 'cannot_trade')
                                                                    tooltipText += 'Not Tradable';
                                                            }
                                                        )

                                                        tooltipText += '</p>';
                                                    }
                                                }
                                            }

                                            tooltipText += '</div>';

                                            $('body').append(tooltipText);

                                            var currentTooltip;

                                            $('#id_slot_' + uniqueId).hover
                                            (
                                                function()
                                                {
                                                    currentTooltip = $('#id_tooltip_' + uniqueId);

                                                    var tooltipWidth  = currentTooltip.width();
                                                    var tooltipHeight = currentTooltip.height();

                                                    var tooltipOffsetLeft = (($(this).width() - tooltipWidth) / 2) - 8;
                                                    var tooltipOffsetTop  = $(this).height() + 10;
                                                    
                                                    var tooltipPositionLeft = $(this).offset().left + tooltipOffsetLeft;
                                                    var tooltipPositionTop  = $(this).offset().top  + tooltipOffsetTop;

                                                    if (tooltipPositionLeft < $(document).scrollLeft())
                                                        tooltipPositionLeft = $(this).offset().left + $(this).width() + 8;

                                                    if ((tooltipPositionLeft + (tooltipWidth  + ($(this).width()  / 4))) - $(document).scrollLeft() > document.documentElement.clientWidth)
                                                        tooltipPositionLeft = $(this).offset().left - (tooltipWidth  + ($(this).width()  / 4) + 2);

                                                    if (((tooltipPositionTop + (tooltipHeight + ($(this).height() / 4))) - $(document).scrollTop()) > document.documentElement.clientHeight)
                                                        tooltipPositionTop  = $(this).offset().top  - (tooltipHeight + ($(this).height() / 4) + 2);

                                                    currentTooltip
                                                        .css('left', tooltipPositionLeft + 'px')
                                                        .css('top',  tooltipPositionTop  + 'px');

                                                    currentTooltip.show();
                                                },
                                                function()
                                                {
                                                    currentTooltip.hide();
                                                }
                                            );
                                        }
                                    }
                                );

                                var nItemsTotal = nItems + nItemsFound;

                                var warningText = 'Warning! ' + (200 - nItemsTotal) + ' item(s) until your backpack is full!';

                                $('#id_summary_warning').html(warningText);

                                $('#id_summary_n_items').text(nItems);

                                $('#id_summary_n_items_found').text(nItemsFound);

                                $('#id_summary_n_unusuals').text(nUnusuals);

                                $('#id_summary_n_hats').text(nHats);

                                var nMetalsWorth = (nScrapMetals * 3) + (nReclaimedMetals * 9) + (nRefinedMetals * 27);

                                $('#id_summary_n_metals').text(nMetals + ' metal(s)' + ' worth ' + nMetalsWorth + ' item(s)' + ' or ' + Math.floor(nMetalsWorth / 81) + ' random hat(s)' + ' or ' + Math.floor(nMetalsWorth / 108) + ' class-specific hat(s)');

                                $('#id_summary_n_crates').text(nCrates);
                                $('#id_summary_n_keys').text(nKeys);

                                $('#id_summary_n_tags').text(nTags);

                                $('#id_summary_n_paints').text(nPaints);

                                $('#id_summary_n_gifts').text(nGifts);

                                $('#id_summary_n_tokens').text(nTokens);

                                $('#id_summary_n_items_total').text(nItemsTotal + ' / 200');

                                if (nItemsFound === 0)
                                    $('#id_items_found_content').hide();
                                else
                                    $('#id_items_found_content').show();
                            }
                        );
                    }
                );
            }
        );

    </script>

    <?php
        $mtime = microtime();
        $mtime = explode(" ", $mtime);
        $mtime = $mtime[1] + $mtime[0];
        $endtime = $mtime;
        $totaltime = ($endtime - $starttime);
        echo "<center>This page was generated in " . $totaltime . " second(s)</center>";
    ?>

</body>

</html>
