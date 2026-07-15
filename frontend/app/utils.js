export function makeHTTPRequest(url) {
  return new Promise((resolve) => {
    const request = new XMLHttpRequest();

    request.addEventListener('load',() => {
      resolve(JSON.parse(request.response));
    });

    request.open('GET',url);
    request.send();
  });
}
