// index_html.h

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Spidey</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #555b61;
      color: #ECF0F1;
      text-align: center;
      margin: 0;
      padding: 0;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      min-height: 100vh;
      position: relative;
      overflow: hidden;
    }

    #container {
      max-width: 900px;
      width: 80%;
      padding: 20px;
      background-color: #434547;
      border-radius: 15px;
      box-shadow: 0 0 50px rgb(255, 255, 255);
      position: relative;
      z-index: 1;
    }

    #command-section {
      margin-bottom: 20px;
    }

    #stream {
      width: 100%;
      max-width: 640px;
      height: auto;
      transform: rotate(180deg);
      border: 5px solid #dacedb;
      border-radius: 10px;
      box-shadow: 0 0 100px rgba(0, 0, 0, 1);
    }

    input[type="text"] {
      padding: 10px;
      width: 100%;
      max-width: 300px;
      border: none;
      border-radius: 5px;
      box-shadow: 0 0 10px rgba(0, 0, 0, 0.2);
    }

    button {
      padding: 10px 20px;
      margin-top: 10px;
      background-color: #b5a5c5;
      border: none;
      border-radius: 5px;
      color: #0c0c0c;
      font-weight: bold;
      cursor: pointer;
      transition: background-color 0.3s ease;
    }

    button:hover {
      background-color: #ffffff;
    }

    #menuButton {
      position: absolute;
      top: 20px;
      left: 20px;
      background-color: #aaadaf;
      color: #0c0c0c;
      padding: 10px;
      border: none;
      border-radius: 5px;
      cursor: pointer;
      font-weight: bold;
      z-index: 2;
    }

    #menuButton:hover {
      background-color: #e5ebf1;
    }

    #popupMenu {
      display: none;
      position: absolute;
      top: 60px;
      left: 20px;
      background-color: #363738;
      padding: 20px;
      border-radius: 10px;
      box-shadow: 0 0 100px rgba(0, 0, 0, 0.5);
      z-index: 3;
    }

    #popupMenu h3 {
      margin-top: 0;
    }

    #closePopup {
      background-color: #ffffff8c;
      color: #0c0c0c;
      padding: 5px 10px;
      border: none;
      border-radius: 5px;
      cursor: pointer;
      font-weight: bold;
    }

    #closePopup:hover {
      background-color: #ffffff;
    }

    .background-animation {
      position: absolute;
      top: 0;
      left: 0;
      width: 100%;
      height: 100%;
      z-index: 0;
      overflow: hidden;
      pointer-events: none;
    }

    @keyframes move {
      0% {
        transform: translateY(0) scale(1);
        opacity: 100;
      }
      100% {
        transform: translateY(-300px) scale(2);
        opacity: 0;
      }
    }

    .circle {
      position: absolute;
      border-radius: 1%;
      background: rgba(255, 255, 255, 0.1);
      animation: move 5s infinite ease-in-out;
    }

    .circle:nth-child(1) {
      width: 50px;
      height: 50px;
      top: 80%;
      left: 5%;
      animation-duration: 10s;
      animation-delay: 0s;
    }

    .circle:nth-child(2) {
      width: 75px;
      height: 75px;
      top: 60%;
      left: 85%;
      animation-duration: 10s;
      animation-delay: 0s;
    }

    .circle:nth-child(3) {
      width: 100px;
      height: 100px;
      top: 40%;
      left: 10%;
      animation-duration: 10s;
      animation-delay: 2s;
    }

    .circle:nth-child(4) {
      width: 150px;
      height: 150px;
      top: 20%;
      left: 90%;
      animation-duration: 11s;
      animation-delay: 3s;
    }

    .circle:nth-child(5) {
      width: 200px;
      height: 200px;
      top: 70%;
      left: 15%;
      animation-duration: 7s;
      animation-delay: 0s;
    }

    .circle:nth-child(6) {
      width: 60px;
      height: 60px;
      top: 50%;
      left: 80%;
      animation-duration: 8s;
      animation-delay: 2s;
    }

    #contactButton {
      position: fixed;
      bottom: 20px;
      right: 20px;
      background-color: #ffffff8e;
      color: #0c0c0c;
      padding: 10px;
      border: none;
      border-radius: 5px;
      cursor: pointer;
      font-weight: bold;
      z-index: 2;
    }

    #contactButton:hover {
      background-color: #f0f0f0;
    }

    #contactPopupMenu {
      display: none;
      position: fixed;
      bottom: 60px;
      right: 20px;
      background-color: #434547;
      padding: 20px;
      border-radius: 10px;
      box-shadow: 0 0 100px rgba(0, 0, 0, 0.5);
      z-index: 1000;
    }

    #contactPopupMenu h3 {
      margin-top: 0;
    }

    #contactPopupMenu a {
      color: #928ad8;
      text-decoration: none;
    }

    #contactPopupMenu a:hover {
      text-decoration: underline;
    }

    #closeContactPopup {
      background-color: #fffbf2;
      color: #0c0c0c;
      padding: 5px 10px;
      border: none;
      border-radius: 5px;
      cursor: pointer;
      font-weight: bold;
      margin-top: 10px;
    }

    #closeContactPopup:hover {
      background-color: #16A085;
    }

    #lightbulbButton {
      position: fixed;
      top: 20px;
      right: 20px;
      background-color: #fad366;
      color: #0c0c0c;
      padding: 10px;
      border: none;
      border-radius: 50%;
      cursor: pointer;
      font-size: 50px;
      z-index: 2;
      transition: background-color 1s ease;
    }

    #lightbulbButton.on {
      background-color: #ecebeb;
      color: #ebe990;
      box-shadow: 0 0 10px 10px rgb(255, 217, 0);
    }

    #lightbulbButton.off {
      background-color: #555b61;
      color: #bdaf8b;
    }

    @media (max-width: 600px) {
      input[type="text"] {
        width: 100%;
        max-width: none;
      }
      button {
        width: 100%;
        max-width: none;
      }
      #stream {
        max-width: 90%;
      }
    }

    #joystick {
  position: fixed;
  right: 100px;
  top: 50%;
  transform: translateY(-50%);
  display: flex;
  flex-direction: column;
  align-items: center;
  z-index: 10;
}

.joystick-row {
  display: flex;
  justify-content: center;
  margin: 5px 0;
}

.joystick-btn {
  width: 80px;
  height: 80px;
  background-color: #8475c9 ;
  box-shadow: 0 0 30px #fefeff;
  border: none;
  border-radius: 50%;
  font-size: 40px;
  font-weight: bold;
  cursor: pointer;
  margin: 5px;
}

.joystick-btn:hover {
  background-color:#fffbfb ;
}

#left-buttons {
  position: fixed;
  left: 100px;
  top: 50%;
  transform: translateY(-50%);
  display: flex;
  flex-direction: column;
  gap: 15px; /* Space between buttons */
}

.side-btn {
  width: 100px;
  padding: 15px;
  background: linear-gradient(135deg, #998de0, #a891d3); /* Gradient effect */
  border: none;
  border-radius: 10px;
  color: rgb(36, 34, 34);
  font-size: 18px;
  font-weight: bold;
  cursor: pointer;
  box-shadow: 0px 4px 15px rgba(0, 0, 0, 0.2); /* Drop shadow */
  transition: transform 0.2s, background-color 0.3s;
}

.side-btn:hover {
  background: linear-gradient(135deg, #5f5f5f, #fffeff); /* Darker gradient on hover */
  transform: scale(1.3); /* Slightly enlarge the button */
}

.side-btn:active {
  background-color: #ffffff; /* Change color when clicked */
  box-shadow: 0px 2px 8px rgba(0, 0, 0, 0.2);
}
#spider {
  position: fixed;
  top: 0;
  left: 0px; /* Adjust to move horizontally */
  z-index: 0; /* Ensure it's behind the content */
  width: 100%; /* Adjust size as needed */
  height: 100%;
  background: none;
}

  </style>
</head>
<body>
  <img id="spider" src="/2765373.jpg" alt="Spider">
  <div class="background-animation">
    <div class="circle"></div>
    <div class="circle"></div>
    <div class="circle"></div>
    <div class="circle"></div>
    <div class="circle"></div>
  </div>

  <button id="menuButton" onclick="toggleMenu()">Notes</button>
  <div id="popupMenu">
    <h3>Note</h3>
    <table style="width: 100%; border-collapse: collapse;">
      <tbody>
        <tr>
          <td style="text-align: left; padding: 8px;">on/off</td>
          <td style="text-align: left; padding: 8px;">- Light on/off</td>
        </tr>
        <tr>
          <td style="text-align: left; padding: 8px;">g</td>
          <td style="text-align: left; padding: 8px;">- Start position</td>
        </tr>
        <tr>
          <td style="text-align: left; padding: 8px;">s</td>
          <td style="text-align: left; padding: 8px;">- Sleep position</td>
        </tr>
        <tr>
          <td style="text-align: left; padding: 8px;">f/b</td>
          <td style="text-align: left; padding: 8px;">- Crawl forward/backward</td>
        </tr>
        <tr>
          <td style="text-align: left; padding: 8px;">wf/wb</td>
          <td style="text-align: left; padding: 8px;">- Walk forward/backward</td>
        </tr>
        <tr>
          <td style="text-align: left; padding: 8px;">dance</td>
          <td style="text-align: left; padding: 8px;"></td>
        </tr>
        <tr>
          <td style="text-align: left; padding: 8px;">dance1</td>
          <td style="text-align: left; padding: 8px;"></td>
        </tr>
        <tr>
          <td style="text-align: left; padding: 8px;">y/n</td>
          <td style="text-align: left; padding: 8px;">- Nod yes/no</td>
        </tr>
      </tbody>
    </table>
    <button id="closePopup" onclick="toggleMenu()">Close</button>
  </div>

<!-- Contact Button -->
<button id="contactButton" onclick="toggleContactMenu()">Contact</button>

<!-- Contact Popup Menu -->
<div id="contactPopupMenu">
    <h3>Contact Details</h3>
    <p>Aditya Karumbaiah</p>
    <p>Email: adityakarumbaiah@gmail.com</p>
    <p>GitHub: <a href="https://github.com/sA-Ge1" target="_blank">https://github.com/sA-Ge1</a></p>
    <button id="closeContactPopup" onclick="toggleContactMenu()">Close</button>
</div>

<!-- Lightbulb Toggle Button -->
<button id="lightbulbButton" onclick="toggleLightbulb()">
  💡
  <!----> 
</button>


  <div id="container">
    <div id="command-section">
      <h2>TERMINAL</h2>
      <input type="text" id="command" placeholder="Enter your command">
      <button onclick="sendText()">Send</button>
      <p id="response"></p>
    </div>

    <h1>---</h1>
    <img id="stream" src="">
  </div>

  <div id="joystick">
    <button class="joystick-btn" id="up" onclick="sendJoystickCommand('wf')">F</button>
    <div class="joystick-row">
      <button class="joystick-btn" id="left" onclick="sendJoystickCommand('l')">L</button>
      <button class="joystick-btn" id="center" onclick="sendJoystickCommand('g')"></button>
      <button class="joystick-btn" id="right" onclick="sendJoystickCommand('r')">R</button>
    </div>
    <button class="joystick-btn" id="down" onclick="sendJoystickCommand('wb')">B</button>
  </div>

  <div id="left-buttons">
    <button id="yesButton" class="side-btn" onclick="sendCommand('y')">Nod</button>
    <button id="noButton" class="side-btn" onclick="sendCommand('n')">Wave</button>
    <button id="danceButton" class="side-btn" onclick="sendCommand('dance')">Dance</button>
    <button id="dance1Button" class="side-btn" onclick="sendCommand('dance1')">Dance</button>
    <button id="sleepButton" class="side-btn" onclick="sendCommand('s')">Sleep</button>
  </div>
  
  

  <script>
    function updateImage() {
      var stream = document.getElementById("stream");
      stream.src = "stream?" + new Date().getTime();  // Update with a timestamp to avoid caching
    }
        // Initialize the button state as "off"
        document.getElementById("lightbulbButton").classList.add("off");

    setInterval(updateImage, 100);  // Refresh the image every 100ms

    function sendText() {
      var xhr = new XMLHttpRequest();
      var command = document.getElementById("command").value;
      xhr.open("GET", "/sendText?data=" + encodeURIComponent(command), true);
      xhr.onreadystatechange = function () {
        if (xhr.readyState == 4 && xhr.status == 200) {
          document.getElementById("response").innerText = "Command Sent: " + command;
          document.getElementById("command").value = "";  // Clear the input field
        }
      };
      xhr.send();
    }

    // Add event listener for Enter key press
    document.getElementById("command").addEventListener("keypress", function(event) {
      if (event.key === "Enter") {
        event.preventDefault();  // Prevent form submission
        sendText();  // Call the sendText function
      }
    });

    function toggleMenu() {
      var menu = document.getElementById("popupMenu");
      if (menu.style.display === "block") {
        menu.style.display = "none";
      } else {
        menu.style.display = "block";
      }
    }

    function toggleContactMenu() {
    var contactMenu = document.getElementById("contactPopupMenu");
    if (contactMenu.style.display === "block") {
        contactMenu.style.display = "none";
    } else {
        contactMenu.style.display = "block";
    }
}

function toggleLightbulb() {
    var button = document.getElementById("lightbulbButton");
    var isOn = button.classList.contains("on");

    if (isOn) {
        // Turn off
        button.classList.remove("on");
        button.classList.add("off");
        sendCommand("off");
    } else {
        // Turn on
        button.classList.remove("off");
        button.classList.add("on");
        sendCommand("on");
    }
}

function sendCommand(command) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/sendText?data=" + encodeURIComponent(command), true);
    xhr.onreadystatechange = function () {
        if (xhr.readyState == 4 && xhr.status == 200) {
            document.getElementById("response").innerText = "Command Sent: " + command;
        }
    };
    xhr.send();
}

function sendJoystickCommand(command) {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/sendText?data=" + encodeURIComponent(command), true);
  xhr.onreadystatechange = function () {
    if (xhr.readyState == 4 && xhr.status == 200) {
      document.getElementById("response").innerText = "Joystick Command Sent: " + command;
    }
  };
  xhr.send();
}

function sendCommand(command) {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/sendText?data=" + encodeURIComponent(command), true);
  xhr.onreadystatechange = function () {
    if (xhr.readyState == 4 && xhr.status == 200) {
      document.getElementById("response").innerText = "Command Sent: " + command;
    }
  };
  xhr.send();
}


  </script>
</body>
</html>


)rawliteral";
