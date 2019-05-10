const char HTTP_INDEX[] PROGMEM = R"=====(
<html>

<title> Text Matrix </title>
<h1> Text Matrix </h1>
<form id="display_form">
    Text: <input type="text" name="message"><br>
    <input type="submit">
</form>

<button onclick="window.location.href = '/settings'"> Settings </button>
<h1> History </h1>
<ul>
    <li>MESSAGE1</li>
    <li>MESSAGE2</li>
    <li>MESSAGE3</li>
    <li>MESSAGE4</li>
    <li>MESSAGE5</li>
</ul>
<script>
document.getElementById('display_form').addEventListener('submit', (e) => {
  e.preventDefault();
  const formData = new FormData(e.target);
  const data = Array.from(formData.entries()).reduce((memo, pair) => ({
    ...memo,
    [pair[0]]: pair[1],
  }), {});
  fetch("/message", {
      headers: {
        "Accept": "application/json",
        "Content-Type": "application/json"
      },
      method: "POST",
      body: JSON.stringify(data)
  });
});
</script>
</html>
)=====";