export async function makeHTTPGetRequest(url) {
  const response = await fetch(url);

  if (!response.ok) {
    throw new Error(`HTTP-Error: ${response.status}`);
  }

  return await response.json();
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

    return await response.json();
  }
  catch (error) {
    console.error("Error during POST request:", error);
    return {"error":"Server error"};
  }
}
