const forwardBtn = document.getElementById('forwardBtn');
// const backwardBtn = document.getElementById('backwardBtn');
const stopBtn = document.getElementById('stopBtn');
const rightBtn = document.getElementById('right-button');
const leftBtn = document.getElementById('left-button');
const concertinaBtn = document.getElementById('concMov');
const ondulatedBtn = document.getElementById('ondMov');


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

forwardBtn.addEventListener('click', () => {
  sendRequest('/forward');
  console.log("going forward");
});

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

concertinaBtn.addEventListener('click', () => {
  sendRequest('/concertina');
  console.log("Concertina Movement");
})

ondulatedBtn.addEventListener('click', () => {
  sendRequest('/ondulated');
  console.log("Ondulated Movement");
})

