function initMap()
{
    // Map options
    var options = {
        zoom:12,
        center:{lat:49.9927, lng:16.4977}
    };

    // New map
    var map = new google.maps.Map(
        document.getElementById("oafl-map"),
        options
    );

    // Add marker
    var marker = new google.maps.Marker({
        position:{lat:49.99256, lng:16.5020714},
        map: map
    });

    // Add info window
    var infoWindow = new google.maps.InfoWindow({
        content: '<div><b>Víceúčelová Hala</b><br>' +
                 'Dolní Dobrouč 73, 561 02<br>' +
                 '<a href="http://www.dolnidobrouc.cz/menus/76-sportovni-hala">dolnidobrouc.cz</a></div>'
    });

    // Open info window by default
    infoWindow.open(map, marker);

    // Open info window by click on the marker
    marker.addListener('click', function() {
        infoWindow.open(map, marker);
    });
}
