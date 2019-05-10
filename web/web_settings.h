const char HTTP_SETTINGS[] PROGMEM = R"=====(
<html>

<title> Text Matrix </title>
<h1> Text Matrix </h1>
<h2> Settings </h2>
<h3> Web Access </h3>
<form id="settings_form">
        Name <input type="text" name="name"><br>
        User      <input type="text" name="web_user"><br>
        Password  <input type="password" name="web_pass"><br>
<h3> Wi-Fi </h3>
    SSID 1      <input type="text" name="ssid1"><br>
    PASSWORD 1 <input type="password" name="pass1"><br>
    SSID 2      <input type="text" name="ssid2"><br>
    PASSWORD 2 <input type="password" name="pass2"><br>
    SSID 3 <input type="text" name="ssid3"><br>
    PASSWORD 3 <input type="password" name="pass3"><br>
    SSID 4 <input type="text" name="ssid4"><br>
    PASSWORD 4 <input type="password" name="pass4"><br>
<h3> Access Point </h3>
        SSID       <input type="text" name="ap_ssid"><br>
        PASSWORD <input type="text" name="ap_pass"><br>
        <input type="submit">
</form>
<button onclick="window.location.href = '/'"> Go back </button>
<script>
fetch("/load")
.then(function(res){return res.json();})
.then(function(data){
    for (form_obj in data){
        document.getElementsByName(form_obj)[0].value = data[form_obj];
    }
});

document.getElementById('settings_form').addEventListener('submit', (e) => {
    e.preventDefault();
    const formData = new FormData(e.target);
    const data = Array.from(formData.entries()).reduce((memo, pair) => ({
        ...memo,
        [pair[0]]: pair[1],
    }), {});
    fetch("/save", {
        headers: {
            "Accept": "application/json",
            "Content-Type": "application/json"
        },
        method: "POST",
        body: JSON.stringify(data)
    });
});

</script>
</html>
)=====";