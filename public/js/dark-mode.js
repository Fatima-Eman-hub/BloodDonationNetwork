// Dark Mode Toggle Module - Add to any HTML file for night light support
// Usage: Include in <head>: <script src="js/dark-mode.js"></script>

(function() {
    // Initialize dark mode on page load
    function initDarkMode() {
        const savedTheme = localStorage.getItem('theme') || 'light';
        if (savedTheme === 'dark') {
            document.body.classList.add('dark-mode');
            updateToggleIcon();
        }
    }

    // Toggle dark mode
    window.toggleDarkMode = function() {
        const body = document.body;
        const icon = document.getElementById('theme-icon');
        
        if (body.classList.contains('dark-mode')) {
            body.classList.remove('dark-mode');
            localStorage.setItem('theme', 'light');
            if (icon) icon.textContent = '🌙';
        } else {
            body.classList.add('dark-mode');
            localStorage.setItem('theme', 'dark');
            if (icon) icon.textContent = '☀️';
        }
    };

    // Update toggle button icon based on current theme
    function updateToggleIcon() {
        const icon = document.getElementById('theme-icon');
        if (icon) {
            const isDarkMode = document.body.classList.contains('dark-mode');
            icon.textContent = isDarkMode ? '☀️' : '🌙';
        }
    }

    // Initialize on DOM ready
    if (document.readyState === 'loading') {
        document.addEventListener('DOMContentLoaded', initDarkMode);
    } else {
        initDarkMode();
    }

    // Also initialize if this script is loaded via defer
    setTimeout(initDarkMode, 100);
})();

// Keyboard shortcut: Alt+D to toggle dark mode
document.addEventListener('keydown', function(e) {
    if (e.altKey && e.key === 'd') {
        window.toggleDarkMode();
    }
});
