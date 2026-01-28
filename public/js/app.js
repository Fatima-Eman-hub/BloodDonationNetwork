/**
 * BloodConnect - Blood Donation System Main Application
 */

const API_BASE_URL = 'http://localhost:18080/api';
window.IS_DEMO_MODE = false;
const IS_DEMO_MODE = window.IS_DEMO_MODE;

const App = {
    user: null,

    init() {
        this.checkAuth();
        this.setupEventListeners();
        this.initAnimations();
    },

    initAnimations() {
        const observer = new IntersectionObserver((entries) => {
            entries.forEach(entry => {
                if (entry.isIntersecting) {
                    entry.target.classList.add('reveal-active');
                    observer.unobserve(entry.target);
                }
            });
        }, { threshold: 0.1 });

        document.querySelectorAll('.reveal').forEach(el => observer.observe(el));
    },

    checkAuth() {
        const userData = localStorage.getItem('blood_connect_user');
        if (userData) {
            this.user = JSON.parse(userData);
        }
    },

    setupEventListeners() {},

    async login(email, password) {
        const tempReg = JSON.parse(localStorage.getItem('blood_connect_temp_reg'));

        if (IS_DEMO_MODE) {
            let userRole = email.includes('donor') ? 'donor' : 'recipient';
            let userName = 'Demo User';
            if (tempReg && tempReg.email === email) {
                userRole = tempReg.role || userRole;
                userName = tempReg.name || userName;
            }
            const demoUser = { success: true, role: userRole, userId: 'DEMO-001', name: userName };
            this.user = demoUser;
            localStorage.setItem('blood_connect_user', JSON.stringify(demoUser));
            this.showToast('Logged in (Demo Mode)', 'success');
            setTimeout(() => this.redirectToDashboard(), 1000);
            return;
        }

        try {
            const response = await fetch(`${API_BASE_URL}/auth/login`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ email, password })
            });

            const data = await response.json();
            if (data.success) {
                this.user = data;
                localStorage.setItem('blood_connect_user', JSON.stringify(data));
                this.redirectToDashboard();
            } else {
                alert(data.message || 'Login failed');
            }
        } catch (error) {
            // Fallback using temp registration data if available
            let fallbackRole = 'donor';
            let fallbackName = 'Ahmed Khan';
            if (tempReg && tempReg.email === email) {
                fallbackRole = tempReg.role;
                fallbackName = tempReg.name;
            }
            const fallbackUser = { success: true, role: fallbackRole, userId: 'FALLBACK-001', name: fallbackName };
            this.user = fallbackUser;
            localStorage.setItem('blood_connect_user', JSON.stringify(fallbackUser));
            this.showToast('Server down. Using local session', 'warning');
            setTimeout(() => this.redirectToDashboard(), 1500);
        }
    },

    redirectToDashboard() {
        if (this.user.role === 'donor') {
            window.location.href = 'dashboard-donor.html';
        } else {
            window.location.href = 'dashboard-recipient.html';
        }
    },

    logout() {
        localStorage.removeItem('blood_connect_user');
        window.location.href = 'index.html';
    },

    // UI Helpers
    showToast(message, type = 'info') {
        const toast = document.createElement('div');
        toast.className = `toast toast-${type}`;
        toast.innerText = message;
        document.body.appendChild(toast);
        setTimeout(() => toast.remove(), 3000);
    },

    // Multi-language Support
    currentLang: 'en',
    translations: {
        'en': {
            'nav-about': 'About',
            'nav-how': 'How It Works',
            'nav-login': 'Login',
            'hero-save': 'Save Lives'
        },
        'ur': {
            'nav-about': 'ہمارے بارے میں',
            'nav-how': 'یہ کیسے کام کرتا ہے',
            'nav-login': 'لاگ ان کریں',
            'hero-save': 'جانیں بچائیں',
            'hero-title-1': 'ہر قطرہ جانیں بچاتا ہے',
            'hero-title-2': 'جانیں بچائیں',
            'hero-subtitle': 'پاکستان کا پہلا سمارٹ خون ملان کرنے والا نظام جو اعلیٰ ڈیٹا کے ڈھانچے اور الگورتھم استعمال کرتے ہوئے فوری جواب کے اوقات کے لیے۔',
            'register-now': 'اب رجسٹر کریں',
            'learn-more': 'مزید جانیں',
            'about-bloodconnect': 'BLOODCONNECT کے بارے میں',
            'innovation-meets': 'جدت و انسانی خدمت ملتی ہے',
            'about-desc': 'BloodConnect صرف ایک ملان کرنے والا نظام نہیں ہے؛ یہ ایک مشن سے چلنے والا نظام ہے۔',
            'custom-dsa': 'اعلیٰ ڈیٹا کے ڈھانچے',
            'real-time': 'حقیقی وقت میں',
            'choose-role': 'اپنا کردار منتخب کریں',
            'choose-subtitle': 'منتخب کریں کہ آپ جانیں بچانے میں کس طرح حصہ ڈالنا چاہتے ہیں',
            'register-donor': 'ڈونر کے طور پر رجسٹر کریں',
            'donor-desc': 'ہیرو بنیں۔ خون عطیہ کریں اور اپنی کمیونٹی میں جانیں بچائیں۔',
            'request-blood': 'خون کی درخواست کریں',
            'recipient-desc': 'فوری ڈونر تلاش کریں اپنی فوری ضروریات کے لیے۔'
        }
    },

    toggleLang(lang) {
        this.currentLang = lang;
        document.querySelectorAll('[data-key]').forEach(el => {
            const key = el.getAttribute('data-key');
            if (this.translations[lang] && this.translations[lang][key]) {
                el.innerText = this.translations[lang][key];
            }
        });

        // Update button states
        const btnEn = document.getElementById('btnEn');
        const btnUr = document.getElementById('btnUr');
        if (btnEn && btnUr) {
            btnEn.classList.toggle('active', lang === 'en');
            btnUr.classList.toggle('active', lang === 'ur');
        }

        localStorage.setItem('blood_lang', lang);
    },

    async updateDonorStatus(donorId, status) {
        if (IS_DEMO_MODE) {
            console.log('Demo Status Update:', status);
            this.showToast(`Status updated to ${status} (Demo)`, 'success');
            return;
        }

        try {
            const response = await fetch(`${API_BASE_URL}/donor/status`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ donorId, status })
            });
            if (response.ok) {
                this.showToast(`Status updated to ${status}`, 'success');
            }
        } catch (error) {
            console.error('Status update error:', error);
            this.showToast('Failed to update status on server', 'error');
        }
    },

    async acceptRequest(donorId, requestId) {
        if (IS_DEMO_MODE) {
            this.showToast('Request Accepted (Demo)', 'success');
            return true;
        }
        try {
            const response = await fetch(`${API_BASE_URL}/donor/accept-request`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ donorId, requestId })
            });
            if (response.ok) {
                this.showToast('Life saved! Request completed.', 'success');
                return true;
            }
        } catch (error) {
            console.error(error);
            this.showToast('Failed to accept request', 'error');
        }
        return false;
    },

    async updateDonorInfo(donorId, data) {
        if (IS_DEMO_MODE) {
            this.showToast('Profile updated (Demo)', 'success');
            return true;
        }
        try {
            const response = await fetch(`${API_BASE_URL}/donor/update`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ donorId, ...data })
            });
            if (response.ok) {
                this.showToast('Profile changes saved!', 'success');
                return true;
            }
        } catch (error) {
            console.error(error);
            this.showToast('Failed to save profile', 'error');
        }
        return false;
    },

    // WebSocket Init
    initWebSocket() {
        const socket = new WebSocket(`ws://${window.location.host}/ws`);
        socket.onopen = () => console.log('WebSocket Connected');
        socket.onmessage = (e) => {
            console.log('Update:', e.data);
            this.showToast('Real-time updates active', 'success');
        };
    }
};

function toggleLang(l) { App.toggleLang(l); }

document.addEventListener('DOMContentLoaded', () => {
    App.init();
    if (localStorage.getItem('blood_lang')) {
        App.toggleLang(localStorage.getItem('blood_lang'));
    }
});
