function sendForm(form, password) {
    var p = document.createElement("input");
    form.appendChild(p);
    p.name = "p";
    p.type = "hidden"

    var pSize = document.createElement("input");
    form.appendChild(pSize);
    pSize.name = "pSize";
    pSize.type = "hidden"
    pSize.value = password.value.length;

    var shaObj = new jsSHA(password.value, "TEXT");
    p.value = shaObj.getHash("SHA-512", "HEX");
    password.value = "";
    form.submit();
}

function getAJAX() {
    var xmlhttp;
    if (window.XMLHttpRequest)
        xmlhttp = new XMLHttpRequest();
    else
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    return xmlhttp;
}

function messageBoxAsk(text, callback) {
    messageBox(text, "yesno", callback);
}

function messageBox(text, messageBoxType, callback) {

    var $body = $("body").first();

    var $mbox = $('<div class="messageBox"/>');
    var $p = $('<p>' + text + '</p>);');
    $mbox.append($p);
    if (arguments.length >= 2) {

        if($("#messageboxOverlay").length == 0)
        {
            var $overlay = $("<div id='messageboxOverlay' />");
            $body.append($overlay);
            $overlay.fadeTo(200, 0.5);
        }

        switch (messageBoxType) {
            case "ok":
                var $rightDiv = $('<div class="messageBoxBottom" />');
                $mbox.append($rightDiv);
                var $okButton = $('<a class="button">Ok</a>');
                $okButton.click(function(){
                    hideMessageBox($mbox);
                });
                $rightDiv.append($okButton);
                break;
            case "yesno":
                var $rightDiv = $('<div class="messageBoxBottom" />');
                $mbox.append($rightDiv);
                var $noButton = $('<a class="button">No</a>');
                $noButton.click(function(){
                    hideMessageBox($mbox);
                });
                $rightDiv.append($noButton);
                var $yesButton = $('<a class="button" style="margin-left: 10px">Yes</a>');
                $yesButton.click(function(){
                    hideMessageBox($mbox);
                    callback();
                });
                $rightDiv.append($yesButton);
                break;
        }

        $body.append($mbox);
        $mbox.css('top', -$mbox.outerHeight() - 10);
        $mbox.animate({
            top: "0"
        }, 800, "easeOutExpo");
    }
    else {
        $body.append($mbox);
        $mbox.css('top', -$mbox.outerHeight() - 10);
        $mbox.animate({
            top: "0"
        }, 800, "easeOutExpo").delay(3000).animate({top: -$mbox.outerHeight() - 10}, 300, "linear", function () {
            $(this).remove();
        });
    }
}

function hideMessageBox($mbox) {
    if ($('.messageBox').length == 1) {
        $("#messageboxOverlay").fadeOut(200, function () {
            $("#messageboxOverlay").remove();
        });
    }
    $mbox.animate({top: -$mbox.outerHeight() - 10}, 300, "linear", function () {
        $mbox.remove();
    });
}

function getQueryParams(qs) {
    qs = qs.split("+").join(" ");
    var params = {},
        tokens,
        re = /[?&]?([^=]+)=([^&]*)/g;

    while (tokens = re.exec(qs)) {
        params[decodeURIComponent(tokens[1])]
            = decodeURIComponent(tokens[2]);
    }
    return params;
}