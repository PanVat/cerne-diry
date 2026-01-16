document.addEventListener("DOMContentLoaded", () => {
    const form = document.getElementById("form");
    if (!form) return;

    form.addEventListener("submit", (e) => {
        e.preventDefault();
        const spin = parseFloat(form.spin.value);
        const mass = parseFloat(form.mass.value);
        const rin = parseFloat(form.rim_in.value);
        const rout = parseFloat(form.rim_out.value);
        const r0 = parseFloat(form.distance.value);
        const theta = parseFloat(form.angle.value);

        console.log({spin, mass, rin, rout, r0, theta});
    });
});