/* AJAX, který umožní odesílání formuláře bez reloadu stránky */
document.addEventListener('DOMContentLoaded', function() {
  const form = document.getElementById('form');
  const output = document.getElementById('output');
  if (!form || !output) return;

  form.addEventListener('submit', function(e) {
    e.preventDefault();
    const formData = new FormData(form);
    const params = new URLSearchParams(formData);

    fetch('/run/get_data.exe', {
      method: 'POST',
      headers: {'Content-Type': 'application/x-www-form-urlencoded'}, /* Důležité, jinak formulář nebude fungovat */
      body: params
    })
      .then(res => res.text())
      .then(text => {
        output.innerHTML = text;
        document.getElementById('screen_img').src = 'img/screen.png?v=' + Date.now();
        document.getElementById('plplot_img').src = 'img/plplot.png?v=' + Date.now();
      })
      .catch(err => alert('Chyba při požadavku na server: ' + err));
  });
});