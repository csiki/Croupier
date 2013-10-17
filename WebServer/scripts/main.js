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

function getAJAX()
{
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

function messageBox(text) {
    var $mbox = $('<div class="messageBox"/>');
    $mbox.html(text);
    $mbox.hide();
    $mbox.appendTo($("body").first());
    $mbox.fadeTo(1, 0).show().animate({
        top: "+=20",
        opacity: 1.0
    }, 1000).delay(3000).fadeOut(600, function () {
            $(this).remove();
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