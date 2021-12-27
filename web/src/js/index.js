var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function () {
  

    // function update() {
    //   var xhr = new XMLHttpRequest();
    //   xhr.open('post', '/set/settings');
    //   xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
    //   xhr.send(
    //     'r=' + r +
    //     '&g=' + g +
    //     '&b=' + b +
    //     '&iT=' + inhale.value +
    //     '&iP=' + inhalePause.value +
    //     '&eT=' + exhale.value +
    //     '&eP=' + exhalePause.value
    //   );
    // }

};

xhttp.open("GET", "/get/settings", true);
xhttp.send();