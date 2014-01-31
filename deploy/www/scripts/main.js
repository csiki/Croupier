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

function messageBoxAsk(text) {
    return confirm(text);
}

function messageBox(text, messageBoxType) {
    if (arguments.length == 2) {
        switch (messageBoxType) {
            case "ok":
                var $mbox = $('<div class="messageBox"/>');
                var $p = $('<p>' + text + '</p>);');
                $mbox.append($p);
                var $rightDiv = $('<div class="messageBoxBottom" />');
                $mbox.append($rightDiv);
                var $okButton = $('<a class="button">Ok</a>');
                $okButton.click(hideMessageBox);
                $rightDiv.append($okButton);

                var overlay = $("<div id='lean_overlay'></div>");

                $("body").first().append($mbox);
                $mbox.css('top', -$mbox.outerHeight() - 10);
                $mbox.animate({
                    top: "0"
                }, 800, "easeOutExpo");
                /*$mbox.animate({top: -$mbox.outerHeight() - 10}, 400, "linear", function () {
                    $(this).remove();
                });*/
                break;
            case "yesno":
                $mbox.animate({
                    top: "0"
                }, 400, "easeOutExpo").delay(3000).animate({top: -$mbox.outerHeight() - 10}, 400, "linear", function () {
                    $(this).remove();
                });
                break;
        }
    }
    else {
        var $mbox = $('<div class="messageBox"/>');
        $mbox.html(text);
        $mbox.appendTo($("body").first());
        $mbox.css('top', -$mbox.outerHeight() - 10);
        $mbox.animate({
            top: "0"
        }, 400, "easeOutExpo").delay(3000).animate({top: -$mbox.outerHeight() - 10}, 400, "linear", function () {
            $(this).remove();
        });
    }
}

function hideMessageBox()
{
    var $mbox = $('.messageBox');
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