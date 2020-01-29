    connection = true;
    //Get Name
    fetch("/name").then(function(response) {
      return response.text().then(function(text) {
        document.getElementById("name").innerHTML = text;
        document.title = text;
      });
    });

    //Ping
    if(location.protocol == "http:") {
      pingInterval = setInterval(ping, 5000);
    }

    //Reboot
    function reboot(){
      show_error("Red&eacute;marrage...");
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
            show_ok("Connect&eacute;");
            clearInterval(pingInterval);
            pingInterval = setInterval(ping, 5000);
          }
        }
      }).catch(function(error) {
        if(connection){
        loading();
        connection = false;
        show_error("D&eacute;connect&eacute;");
        clearInterval(pingInterval);
        pingInterval = setInterval(ping, 1000);
        }
        show_error("Reconnexion...");
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
        show_ok("Message envoy&eacute;");
      });
    });
  }

