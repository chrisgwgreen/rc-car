#include <html.h>
HTML::HTML() {}
const char* HTML::getHTML() {
    return R"rawliteral(<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>RC Remote</title>
    <style>
      html,
      body {
        height: 100%;
        overflow: hidden;
        user-select: none;
        touch-action: manipulation;
      }

      body {
        background: #292f37;
      }

      .chevron-left {
        top: 50%;
        left: 10%;
        transform: translateY(-50%) rotate(135deg);
      }

      #wrapper {
        width: 80vw;
        height: 80vw;
        max-height: 550px;
        max-width: 550px;
        background: linear-gradient(to bottom, #fe8768, #a24982);
        border-radius: 50%;
        position: absolute;
        left: 50%;
        top: 10%;
        transform: translateX(-50%);
      }

      .arrow-container {
        position: absolute;
        top: 2vw;
        left: 2vw;
        bottom: 2vw;
        right: 2vw;
        background: #292f37;
        border-radius: 50%;
        display: flex;
        align-items: center;
        justify-content: center;
      }

      .cheron {
        position: absolute;
        width: 6vw;
        height: 6vw;
        border-bottom: 1vw solid;
        border-right: 1vw solid;
        border-color: #393d48;
        opacity: 0.5;
      }

      .chevron-up {
        top: 10%;
        left: 50%;
        transform: translateX(-50%) rotate(-135deg);
      }

      .chevron-right {
        top: 50%;
        right: 10%;
        transform: translateY(-50%) rotate(-45deg);
      }

      .chevron-down {
        bottom: 10%;
        left: 50%;
        transform: translateX(-50%) rotate(45deg);
      }

      #control {
        background: #393d48;
        box-shadow: 0 0 20px #000000b5;
        width: 30vw;
        height: 30vw;
        max-width: 170px;
        max-height: 170px;
        border-radius: 50%;
        cursor: pointer;
      }
    </style>
  </head>

  <body>
    <div id="wrapper">
      <div class="arrow-container">
        <div class="cheron chevron-up"></div>
        <div class="cheron chevron-right"></div>
        <div class="cheron chevron-down"></div>
        <div class="cheron chevron-left"></div>
        <div id="control"></div>
      </div>
    </div>
    <script>
      const gateway = `ws://${window.location.hostname}/ws`;
      const wrapper = document.getElementById("wrapper");
      const control = document.getElementById("control");
      const isTouch = "ontouchstart" in window;

      let halfWrapperDimension;
      let x = 0;
      let y = 0;
      let startScreenX = 0;
      let startScreenY = 0;
      let websocket;

      const handleInteractionStart = (e) => {
        startScreenX = isTouch ? e.touches[0].screenX : e.screenX;
        startScreenY = isTouch ? e.touches[0].screenY : e.screenY;

        !isTouch &&
          control.addEventListener("mousemove", handleInteractionMove);
        !isTouch && control.addEventListener("mouseup", handleInteractionEnd);

        isTouch && control.addEventListener("touchmove", handleInteractionMove);
        isTouch && control.addEventListener("touchend", handleInteractionEnd);
        isTouch && control.addEventListener("touchcanel", handleInteractionEnd);
      };

      const handleInteractionMove = (e) => {
        const screenX = isTouch ? e.touches[0].screenX : e.screenX;
        const screenY = isTouch ? e.touches[0].screenY : e.screenY;

        x = screenX - startScreenX;
        y = screenY - startScreenY;

        x =
          Math.abs(x) < halfWrapperDimension
            ? x
            : Math.sign(x) * halfWrapperDimension;
        y =
          Math.abs(y) < halfWrapperDimension
            ? y
            : Math.sign(y) * halfWrapperDimension;

        render(x, y);
      };

      const render = (x, y) => {
        control.setAttribute("style", `transform: translate(${x}px, ${y}px)`);
        websocket.send(`${Math.floor((x / halfWrapperDimension) * 100)},${-Math.floor((y / halfWrapperDimension) * 100)}`);
      };

      const handleInteractionEnd = () => {
        !isTouch &&
          control.removeEventListener("mousemove", handleInteractionMove);
        !isTouch &&
          control.removeEventListener("mouseup", handleInteractionEnd);

        isTouch &&
          control.removeEventListener("touchmove", handleInteractionMove);
        isTouch &&
          control.removeEventListener("touchend", handleInteractionEnd);
        isTouch &&
          control.removeEventListener("touchcanel", handleInteractionEnd);

        x = 0;
        y = 0;
        render(x, y);
      };

      const handleResize = () => {
        halfWrapperDimension = wrapper.getBoundingClientRect().width / 2;
      };

      const handleOpen = () => {
        console.log("Connection opened");
      }

      const handleClose = () => {
        console.log("Connection closed");
        setTimeout(handleInitWebSocket, 2000);
      }

      const handleInitWebSocket = () => {
        console.log("Trying to open a WebSocket connection...");
        websocket = new WebSocket(gateway);
        websocket.onopen = handleOpen;
        websocket.onclose = handleClose;
      };

      handleResize();

      !isTouch && control.addEventListener("mousedown", handleInteractionStart);
      isTouch && control.addEventListener("touchstart", handleInteractionStart);

      window.addEventListener("resize", handleResize);
      window.addEventListener("load", handleInitWebSocket);
    </script>
  </body>
</html>
)rawliteral";}
