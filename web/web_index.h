const char HTTP_INDEX[] PROGMEM = R"=====(
<html>

<head>
<meta name="viewport" http-equiv="Content-Type" content="text/html; charset=UTF-8, width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
<title> Text Matrix </title>

<style>

  h1{
    background-color:#1F1F1F;
    color: #E2E2E2;
    text-align: center;
    font-family: "Arial";
  }
  input{
    height:60px;
    width:70%;
    color:#ff5151;
    background-color:#2D2D2D;
    border:none;
    font-size:15pt;
    font-weight: bold;
    vertical-align: bottom;
    text-indent:10px;
  }
  button{
    width:30%;
    height:60px;
    border: 0px solid;
    border-radius: 4px;
    color: #1f1f1f;
    font-size: 25px;
    font-weight: bold;
    background-color:#ffaa1c;
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
    background-color:#ffaa1c;
    color:#000000;
    border: 0px solid;
    border-color:#121212;
  }
  td{
    border: 1px solid;
    border-color:#121212;
  }


</style>
</head>
<body>
<h1><span>&#128223;</span> <span id="name">...</span></h1>
<form id="display_form">
    <input type="text" placeholder="Write Text Here" name="message"> <button style="border-radius:0px;float: right;" type="submit">Display</button>
</form>
<div style="text-align: center">
<button onclick="window.location.href = '/settings'"> &#128295; Settings </button>
<button onclick="window.location.href = '/update'"> &#128190; Update </button>
<button onclick="window.location.href = ''"> &#128260; Restart </button>
</div>
<h1> &#128203; History </h1>
<table>
    <tr><th>Date</th><th>Message</th></tr>
    <tr><td>19/05 - 12h20</td><td>This is the last Message sent Lipsum Dolorum Truc Muche</td></tr>
    <tr><td>19/05 - 13h00</td><td>Other Message</td></tr>
    <tr><td>19/04 - 11h00</td><td>Some message stuff</td></tr>
    <tr><td>19/04 - 11h00</td><td>Some message stuff</td></tr>
    <tr><td>19/04 - 11h00</td><td>Some message stuff</td></tr>
    <tr><td>19/04 - 11h00</td><td>Some message stuff</td></tr>
</table>
<script>
fetch("/name").then(function(response) {
    return response.text().then(function(text) {
      document.getElementById("name").innerHTML = text;
      document.title = text;
    });
  });

document.getElementById('display_form').addEventListener('submit', (e) => {
  e.preventDefault();
  const formData = new FormData(e.target);
  const data = Array.from(formData.entries()).reduce((memo, pair) => ({
    ...memo,
    [pair[0]]: pair[1],
  }), {});
  fetch("/message", {
      headers: {
        "Accept": "application/json",
        "Content-Type": "application/json"
      },
      method: "POST",
      body: JSON.stringify(data)
  });
});
</script>
</body>
</html>
)=====";