function sendForm(form, password)
{
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