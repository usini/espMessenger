    connection = true;
    //Get Name
    settings = {};   
    getSettings();

    //Ping
    if(location.protocol == "http:") {
      pingInterval = setInterval(ping, 5000);
    }

    //Reboot
    function reboot(){
      show_error(txt_restart);
      loading();
      clearInterval(pingInterval);
      pingInterval = setInterval(ping, 100);
      fetch("/reboot");
    }

    //Ping
    function ping(){
      fetch("/ping")
      .then(function(response) {
        if(response.ok){
        if(!connection){
            connection = true;
            hide_loading();
            show_ok(txt_connected);
            clearInterval(pingInterval);
            pingInterval = setInterval(ping, 5000);
          }
        }
      }).catch(function(error) {
        if(connection){
        loading();
        connection = false;
        show_error(txt_disconnected);
        clearInterval(pingInterval);
        pingInterval = setInterval(ping, 1000);
        }
        show_error(txt_reconnection);
      });
    }

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

    function manage_index_form(){
        //Display Form
        document.getElementById('display_form').addEventListener('submit', (e) => {
      console.log("Loading");
      loading();
      e.preventDefault();
      const formData = new FormData(e.target);
      const data = Array.from(formData.entries()).reduce((memo, pair) => ({
        ...memo,
        [pair[0]]: pair[1],
      }), {});
      console.log(data);
      fetch("/message/"+data.message).then(function(){
        console.log("Hide");
        hide_loading();
        show_ok(txt_message_sent);
      });
    });
  }

  function changeSpeed(value){
    fetch("/speed/" + value);
  }

  function changeIntensity(value){
    fetch("/intensity/" + value);
  }

  function getSettings(){
    fetch('/settings')
    .then(res => res.json())
    .then(function(json){
      settings = json;
      document.getElementById("name").innerHTML = settings.name;
      document.title = settings.name;
      document.getElementById("speed").value = settings.speed;
      document.getElementById("intensity").value = settings.intensity;
    });
  }
