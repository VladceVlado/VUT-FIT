// ToDo:
// - history push state (for changing url)
$(document).ready(function() {
    $(".tr-clickable").click(function() {
        let dataDetail = $(this).attr("data-detail-id");
        let lineAfter = $("#" + dataDetail);
        let wasVisible = lineAfter.is(":visible");
        $(".tr-clickable + tr").hide();
        if (!wasVisible) {
            lineAfter.show();
        }
    });
});
