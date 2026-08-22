export function makeHTTPGetRequest(url) {
  return new Promise((resolve) => {
    const request = new XMLHttpRequest();

    request.addEventListener('load',() => {
      resolve(JSON.parse(request.response));
    });

    request.open('GET',url);
    request.send();
  });
}

export async function makeHTTPPostRequest(url, data) {
  // Without nuxt may send a second request
  if(!import.meta.client) return;

  try {
    const response = await fetch(url, {
      method: "POST",
      headers: {
        "Content-Type": "application/json"
      },
      body: JSON.stringify(data)
    });
  }
  catch (error) {
    console.error("Error during POST request:", error);
  }
}
