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

function messageBoxAsk(text) {
    return confirm(text);
}

function messageBox(text) {
    var p = document.createElement("div");
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

function participate_bot(id, isAdd, complete) {
    var xmlhttp;
    if (window.XMLHttpRequest)
        xmlhttp = new XMLHttpRequest();
    else
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    xmlhttp.onreadystatechange = function () {
        if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
            complete.call();
        }
    }
    xmlhttp.open("GET", "participate_bot.php?botid=" + id + "&val=" + (isAdd ? "1" : "0"), true);
    xmlhttp.send();
}

function deleteBot(id) {
    var xmlhttp;
    if (window.XMLHttpRequest)
        xmlhttp = new XMLHttpRequest();
    else
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    xmlhttp.onreadystatechange = function () {
        if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
            if (xmlhttp.responseText == "1") {
                $("#bot" + id).children('td')
                    .animate({ 'padding-top': 0, 'padding-bottom': 0}, 100, "linear", function () {
                        $(this)
                            .wrapInner('<div />')
                            .children()
                            .slideUp(200, function () {
                                $(this).closest('tr').remove();
                            });
                    });
            }
        }
    }
    xmlhttp.open("GET", "delete_bot.php?botid=" + id, true);
    xmlhttp.send();
}