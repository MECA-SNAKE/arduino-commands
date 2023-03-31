const forwardBtn = document.getElementById('forwardBtn');
const backwardBtn = document.getElementById('backwardBtn');
const stopBtn = document.getElementById('stopBtn');
const rightBtn = document.getElementById('right-button');
const leftBtn = document.getElementById('left-button');

const ESP8266_IP = '172.20.10.10';

function sendRequest(path) {
  fetch(`http://${ESP8266_IP}/${path}`)
    .then(response => {
      if (response.status === 200) {
        console.log(`Request successful: ${path}`);
      } else {
        console.error(`Error sending request: ${response.status}`);
      }
    })
    .catch(err => console.error(`Error sending request: ${err}`));
}

function sendMotorSpeedRequest(speed) {
  const percent = Math.round(speed / 100 * 255);
  sendRequest(`speed?percent=${percent}`);
  speedOutput.innerHTML = `${speed}%`;
}

forwardBtn.addEventListener('click', () => {
  sendRequest('/forward');
  console.log("hola");
});

backwardBtn.addEventListener('click', () => {
  sendRequest('/backward');
  console.log("hola");
});

rightBtn.addEventListener('click', () => {
  sendRequest('/right');
  console.log("hola");
});

leftBtn.addEventListener('click', () => {
  sendRequest('/left');
  console.log("hola");
});

stopBtn.addEventListener('click', () => {
  sendRequest('/stop');
  console.log("hola");
});
