function sendForm(form, password) {
    var $pass = $('<input name="p" type="hidden"/>');
    $(form).append($pass);

    //need this, because password length is lost after encode

    var $pLength = $('<input name="pLength" type="hidden" />');
    $pLength.val(password.value.length);
    $(form).append($pLength);

    var shaObj = new jsSHA(password.value, "TEXT");
    $pass.val(shaObj.getHash("SHA-512", "HEX"));
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

function createBaseMessageBox(text) {
    var $mbox = $('<div class="messageBox"/>');
    var $p = $('<p>' + text + '</p>);');
    $mbox.append($p);
    return $mbox;
}

function messageBox(text) {
    var $mbox = createBaseMessageBox(text);
    $("body").first().append($mbox);
    $mbox.css('top', -$mbox.outerHeight() - 10);
    $mbox.animate({
        top: "0"
    }, 800, "easeOutExpo").delay(3000).animate({top: -$mbox.outerHeight() - 10}, 300, "linear", function () {
        $(this).remove();
    });
}

function messageBoxOk(text, callback) {
    var $mbox = createBaseMessageBox(text);
    $mbox.attr('data-modal', true);
    var $rightDiv = $('<div class="messageBoxBottom" />');
    $mbox.append($rightDiv);
    var $okButton = $('<a class="button">Ok</a>');
    $okButton.click(function () {
        hideMessageBox($mbox);
        callback();
    });
    $rightDiv.append($okButton);

    showOverlay();
    $("body").first().append($mbox);
    $mbox.css('top', -$mbox.outerHeight() - 10);
    $mbox.animate({
        top: "0"
    }, 800, "easeOutExpo");
}

function messageBoxYesNo(text, yes, no, callback) {
    var $mbox = createBaseMessageBox(text);
    $mbox.attr('data-modal', true);
    var $rightDiv = $('<div class="messageBoxBottom" />');
    $mbox.append($rightDiv);
    var $noButton = $('<a class="button">' + no + '</a>');
    $noButton.click(function () {
        hideMessageBox($mbox);
    });
    $rightDiv.append($noButton);
    var $yesButton = $('<a class="button" style="margin-left: 10px">' + yes + '</a>');
    $yesButton.click(function () {
        hideMessageBox($mbox);
        callback();
    });
    $rightDiv.append($yesButton);

    showOverlay();
    $("body").first().append($mbox);
    $mbox.css('top', -$mbox.outerHeight() - 10);
    $mbox.animate({
        top: "0"
    }, 800, "easeOutExpo");
}

function showOverlay() {
    if ($("#messageboxOverlay").length == 0) {
        var $overlay = $("<div id='messageboxOverlay' />");
        $("body").first().append($overlay);
        $overlay.fadeTo(200, 0.5);
    }
}

function hideOverlay() {
    var $numModals = 0;
    $('.messageBox').each(function (i) {
        if ($(this).attr('data-modal'))
            $numModals++;
    });
    if ($numModals == 1) {
        $("#messageboxOverlay").fadeOut(200, function () {
            $("#messageboxOverlay").remove();
        });
    }
}

function hideMessageBox($mbox) {
    hideOverlay();
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

function addLoading(element) {
    var $e = $(element);
    $e.empty();
    $e.append($('<div class="icon loadingIcon" ></div>'));
}