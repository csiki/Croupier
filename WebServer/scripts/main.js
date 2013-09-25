function sendForm(form, password)
{
    var p = document.createElement("input");
    form.appendChild(p);
    p.name = "p";
    p.type = "hidden"
    var shaObj = new jsSHA(password.value, "TEXT");
    p.value = shaObj.getHash("SHA-512", "HEX");
    password.value = "";
    form.submit();
}