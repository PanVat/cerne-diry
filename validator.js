document.addEventListener("DOMContentLoaded", () => {
    /* Načte formulář z HTML */
    const form = document.getElementById("form");
    /* Po stisku tlačítka načte hodnoty z formuláře */
    form.addEventListener("submit", (e) => {
        const spin = parseFloat(form.spin.value);
        const mass = parseFloat(form.mass.value);
        const rin = parseFloat(form.rim_in.value);
        const rout = parseFloat(form.rim_out.value);
        const temperature = parseFloat(form.temperature.value);
        const r0 = parseFloat(form.distance.value);
        const theta = parseFloat(form.angle.value);

        /* Pole, které uchovává chyby */
        let errors = [];

        /* Kontrola jednotlivých hodnot */
        if (spin < 0 || spin > 1) {
            errors.push("Spin (a) musí být mezi 0 a 1.");
        }

        if (mass < 1) {
            errors.push("Hmotnost musí být větší nebo rovna 1.");
        }

        if (rin < 1) {
            errors.push("r_in musí být ≥ 1.");
        }

        if (rout <= rin) {
            errors.push("r_out musí být větší než r_in.");
        }

        if (temperature < 1) {
            errors.push("Teplota musí být větší než 1°K.");
        }

        if (r0 < 1000) {
            errors.push("r0 musí být alespoň 1000.");
        }

        if (theta < 0 || theta > 90) {
            errors.push("Úhel θ musí být mezi 0° a 90°.");
        }

        if (errors.length > 0) {
            /* Zabrání odeslání formuláře */
            e.preventDefault();
            alert(errors.join("\n"));
        }
    });
});
