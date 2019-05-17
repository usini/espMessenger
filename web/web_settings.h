const char HTTP_SETTINGS[] PROGMEM = R"=====(
<html>

<head>
<meta name="viewport" http-equiv="Content-Type" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
<title> Text Matrix </title>

<style>

    h1{
      background-color:#1F1F1F;
      color: #E2E2E2;
      text-align: center;
      font-family: "Arial";
    }
    h2{
      background-color:#1F1F1F;
      color: #E2E2E2;
      text-align: center;
      font-family: "Arial";
    }
    input{
      height:60px;
      width:80%;
        font-size:15pt;
        font-weight: bold;
    }
    button{
      height:60px;
      border: 0px solid;
      border-radius: 4px;
      color: #000000;
      font-size: 25px;
      font-weight: bold;
      background-color:#6200EE;
      margin-bottom:5px;
      -webkit-transition-duration: 0.1s;
      transition-duration: 0.1s;
    }
    button:hover{opacity:0.6;}
    button:active{transform:scale(1.2,1.2);}
    textarea{
      width:100%;
      text-align:center;
      font-size: 32px;
    }
    body{
      background-color:#121212;
      color:#E2E2E2;
    }
    table{
      width:100%;
      font-size:24px;
      background-color:#2E2E2E;
      color:#f7f7f7;
      border: 1px solid;
      border-color:#121212;
    }
    th{
      background-color:#6200EE;
      color:#000000;
      border: 1px solid;
      border-color:#121212;
    }
    td{
      border: 1px solid;
      border-color:#121212;
    }

  </style>
</head>

<title> Text Matrix </title>
<h1 id="name"> Text Matrix </h1>
<h2> Settings </h2>
<h3> Web Access </h3>
<form id="settings_form">
        Name<br> <input type="text" name="name"><br>
        User<br>      <input type="text" name="web_user"><br>
        Password<br>  <input type="password" name="web_pass"><br>
<h3> Wi-Fi </h3>
    SSID 1<br>      <input type="text" name="ssid1"><br>
    PASSWORD 1<br> <input type="password" name="pass1"><br>
    SSID 2<br>      <input type="text" name="ssid2"><br>
    PASSWORD 2<br> <input type="password" name="pass2"><br>
    SSID 3<br> <input type="text" name="ssid3"><br>
    PASSWORD 3<br> <input type="password" name="pass3"><br>
    SSID 4<br> <input type="text" name="ssid4"><br>
    PASSWORD 4<br> <input type="password" name="pass4"><br>
<h3> Access Point </h3>
        SSID<br>       <input type="text" name="ap_ssid"><br>
        PASSWORD<br> <input type="text" name="ap_pass"><br>
        <input type="submit">
</form>
<button onclick="window.location.href = '/'"> Go back </button>
<script>
fetch("/name").then(function(response) {
    return response.text().then(function(text) {
      document.getElementById("name").innerHTML = text;
      document.title = text;
    });
  });

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