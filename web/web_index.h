const char HTTP_INDEX[] PROGMEM = R"=====(
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
    
    /* Absolute Center Spinner */
    .loading {
      position: fixed;
      z-index: 999;
      height: 2em;
      width: 2em;
      overflow: show;
      margin: auto;
      top: 0;
      left: 0;
      bottom: 0;
      right: 0;
      opacity: 0;
      display:none;
      transition: .8s;
    }
    
    /* Transparent Overlay */
    .loading:before {
      content: '';
      display: block;
      position: fixed;
      top: 0;
      left: 0;
      width: 100%;
      height: 100%;
      background: rgba(20, 20, 20,.8);
    }
    
    .loading:after {
      content: '';
      display: block;
      font-size: 20px;
      width: 1em;
      height: 1em;
      animation: spinner 500ms infinite linear;
      border-radius: 0.5em;
      box-shadow: rgba(255,128,0, 0.75) 1.5em 0 0 0,
      rgba(0,0,0, 0.75) 1.1em 1.1em 0 0,
      rgba(255,128,0, 0.75) 0 1.5em 0 0,
      rgba(0,0,0, 0.75) -1.1em 1.1em 0 0,
      rgba(255,128,0, 0.75) -1.5em 0 0 0,
      rgba(0,0,0, 0.75) -1.1em -1.1em 0 0,
      rgba(255,128,0, 0.75) 0 -1.5em 0 0,
      rgba(0,0,0, 0.75) 1.1em -1.1em 0 0;
    }
    
    /* Animation */
    @keyframes spinner {
      0% {
        transform: rotate(0deg);
      }
      100% {
        transform: rotate(360deg);
      }
    }
    
    
  </style>
  
  
  
</head>
<body>
  <h1><span>&#128223;</span> <span id="name">...</span></h1>
  <div id="loading" class="loading"></div>
  <form id="display_form">
    <input type="text" placeholder="Write Text Here" name="message"> <button style="border-radius:0px;float: right;" type="submit">Display</button>
  </form>
  <div style="text-align: center">
    <button onclick="window.location.href = '/settings'"> &#128295; Settings </button>
    <button onclick="window.location.href = '/update'"> &#128190; Update </button>
    <button onclick="reboot()"> &#128260; Restart </button>
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
    "use strict";var iqwerty=iqwerty||{};iqwerty.toast=function(){function t(o,r,i){if(null!==e())t.prototype.toastQueue.push({text:o,options:r,transitions:i});else{t.prototype.Transitions=i||n;var a=r||{};a=t.prototype.mergeOptions(t.prototype.DEFAULT_SETTINGS,a),t.prototype.show(o,a),a=null}}function e(){return i}function o(t){i=t}var r=400,n={SHOW:{"-webkit-transition":"opacity "+r+"ms, -webkit-transform "+r+"ms",transition:"opacity "+r+"ms, transform "+r+"ms",opacity:"1","-webkit-transform":"translateY(-100%) translateZ(0)",transform:"translateY(-100%) translateZ(0)"},HIDE:{opacity:"0","-webkit-transform":"translateY(150%) translateZ(0)",transform:"translateY(150%) translateZ(0)"}},i=null;return t.prototype.DEFAULT_SETTINGS={style:{main:{background:"rgba(0, 0, 0, .85)","box-shadow":"0 0 10px rgba(0, 0, 0, .8)","border-radius":"3px","z-index":"99999",color:"rgba(255, 255, 255, .9)",padding:"10px 15px","max-width":"60%",width:"100%","word-break":"keep-all",margin:"0 auto","text-align":"center",position:"fixed",left:"0",right:"0",bottom:"0","-webkit-transform":"translateY(150%) translateZ(0)",transform:"translateY(150%) translateZ(0)","-webkit-filter":"blur(0)",opacity:"0"}},settings:{duration:4e3}},t.prototype.Transitions={},t.prototype.toastQueue=[],t.prototype.timeout=null,t.prototype.mergeOptions=function(e,o){var r=o;for(var n in e)r.hasOwnProperty(n)?null!==e[n]&&e[n].constructor===Object&&(r[n]=t.prototype.mergeOptions(e[n],r[n])):r[n]=e[n];return r},t.prototype.generate=function(r,n){var i=document.createElement("div");"string"==typeof r&&(r=document.createTextNode(r)),i.appendChild(r),o(i),i=null,t.prototype.stylize(e(),n)},t.prototype.stylize=function(t,e){Object.keys(e).forEach(function(o){t.style[o]=e[o]})},t.prototype.show=function(o,r){this.generate(o,r.style.main);var n=e();document.body.insertBefore(n,document.body.firstChild),n.offsetHeight,t.prototype.stylize(n,t.prototype.Transitions.SHOW),n=null,clearTimeout(t.prototype.timeout),t.prototype.timeout=setTimeout(t.prototype.hide,r.settings.duration)},t.prototype.hide=function(){var o=e();t.prototype.stylize(o,t.prototype.Transitions.HIDE),clearTimeout(t.prototype.timeout),o.addEventListener("transitionend",t.prototype.animationListener),o=null},t.prototype.animationListener=function(){e().removeEventListener("transitionend",t.prototype.animationListener),t.prototype.destroy.call(this)},t.prototype.destroy=function(){var r=e();if(document.body.removeChild(r),r=null,o(null),t.prototype.toastQueue.length>0){var n=t.prototype.toastQueue.shift();t(n.text,n.options,n.transitions),n=null}},{Toast:t}}(),"undefined"!=typeof module&&(module.exports=iqwerty.toast);
  </script>
  
  <script>
    connection = true;

    fetch("/name").then(function(response) {
      return response.text().then(function(text) {
        document.getElementById("name").innerHTML = text;
        document.title = text;
      });
    });

    pingInterval = setInterval(ping, 5000);

    function reboot(){
      show_error("Restarting...");
      loading();
      clearInterval(pingInterval);
      pingInterval = setInterval(ping, 100);
      fetch("/reboot");
    }

    function ping(){
      fetch("/ping")
      .then(function(response) {
        if(response.ok){
        if(!connection){
            connection = true;
            hide_loading();
            show_ok("Connection OK!");
            clearInterval(pingInterval);
            pingInterval = setInterval(ping, 5000);
          }
        }
      }).catch(function(error) {
        if(connection){
        loading();
        connection = false;
        show_error("Connection Lost");
        clearInterval(pingInterval);
        pingInterval = setInterval(ping, 1000);
        }
        show_error("Reconnecting...");
      });
    }

    document.getElementById('display_form').addEventListener('submit', (e) => {
      console.log("Loading");
      loading();
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
      }).then(function(){
        console.log("Hide");
        hide_loading();
        show_ok("Message sent");
      });
    });

    function loading() {
      elem = document.getElementById("loading");

      requestAnimationFrame(() => elem.style.opacity = 1);
      elem.style.display = "block";
    };
    function hide_loading() {
      elem = document.getElementById("loading");
      elem.style.display = "none";
      requestAnimationFrame(() => elem.style.opacity = 0);
    };

    function show_ok(message){
      var options = {
        style: {
          main: {
            'font-size': "40px",
            background: "#8dd892",
            color: "#fbfbfb"
          }
        }
      };
      iqwerty.toast.Toast(message,options);
    }

    function show_error(message){
      var options = {
        style: {
          main: {
            'font-size': "40px",
            background: "#ff4646",
            color: "#fbfbfb"
          }
        }
      };
      iqwerty.toast.Toast(message,options);
    }

  </script>
</body>
</html>
)=====";