document.getElementById('form').addEventListener('submit', function(e) {
  e.preventDefault();

  fetch('/execute.bat', { method: 'POST' })
    .then(res => res.text())
    .then(text => {
      // Výstup z bat/sh souboru
      console.log('Výstup serverového skriptu:\n' + text);
      document.getElementById('output').src = 'img/screen.png?v=' + Date.now();
    })
    .catch(err => alert('Chyba při požadavku na server: ' + err));
});