const forwardBtn = document.getElementById('forwardBtn');
// const backwardBtn = document.getElementById('backwardBtn');
const stopBtn = document.getElementById('stopBtn');
const rightBtn = document.getElementById('right-button');
const leftBtn = document.getElementById('left-button');

const SNAKE_IP = 'enter IP address here';

function sendRequest(path) {
  fetch(`http://${SNAKE_IP}/${path}`)
    .then(response => {
      if (response.status === 200) {
        console.log(`Request successful: ${path}`);
      } else {
        console.error(`Error sending request: ${response.status}`);
      }
    })
    .catch(err => console.error(`Error sending request: ${err}`));
}

// function sendMotorSpeedRequest(speed) {
//   const percent = Math.round(speed / 100 * 255);
//   sendRequest(`speed?percent=${percent}`);
//   speedOutput.innerHTML = `${speed}%`;
// }

forwardBtn.addEventListener('click', () => {
  sendRequest('/forward');
  console.log("going forward");
});

// backwardBtn.addEventListener('click', () => {
//   sendRequest('/backward');
//   console.log("hola");
// });

rightBtn.addEventListener('click', () => {
  sendRequest('/right');
  console.log("Going right");
});

leftBtn.addEventListener('click', () => {
  sendRequest('/left');
  console.log("Going Left");
});

stopBtn.addEventListener('click', () => {
  sendRequest('/stop');
  console.log("Stopping");
});
