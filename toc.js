// Populate the sidebar
//
// This is a script, and not included directly in the page, to control the total size of the book.
// The TOC contains an entry for each page, so if each page includes a copy of the TOC,
// the total size of the page becomes O(n**2).
class MDBookSidebarScrollbox extends HTMLElement {
    constructor() {
        super();
    }
    connectedCallback() {
        this.innerHTML = '<ol class="chapter"><li class="chapter-item affix "><a href="index.html">Welcome to Gwion</a></li><li class="chapter-item "><a href="Overview/index.html"><strong aria-hidden="true">1.</strong> Overview</a><a class="toggle"><div>❱</div></a></li><li><ol class="section"><li class="chapter-item "><a href="Overview/First_Steps/index.html"><strong aria-hidden="true">1.1.</strong> First Steps</a><a class="toggle"><div>❱</div></a></li><li><ol class="section"><li class="chapter-item "><a href="Overview/First_Steps/InstallingGwion.html"><strong aria-hidden="true">1.1.1.</strong> Installing gwion</a></li><li class="chapter-item "><a href="Overview/First_Steps/Configure.html"><strong aria-hidden="true">1.1.2.</strong> Configuring Gwion</a></li><li class="chapter-item "><a href="Overview/First_Steps/InstallinOnWindows.html"><strong aria-hidden="true">1.1.3.</strong> Gwion On Windows</a></li></ol></li><li class="chapter-item "><a href="Overview/declaration.html"><strong aria-hidden="true">1.2.</strong> Declarations</a></li><li class="chapter-item "><a href="Overview/globalvalues.html"><strong aria-hidden="true">1.3.</strong> Global Values</a></li><li class="chapter-item "><a href="Overview/Keywords.html"><strong aria-hidden="true">1.4.</strong> Keywords</a></li><li class="chapter-item "><a href="Overview/SpecialWords.html"><strong aria-hidden="true">1.5.</strong> Special Values</a></li><li class="chapter-item "><a href="Overview/Testing.html"><strong aria-hidden="true">1.6.</strong> Tests</a></li></ol></li><li class="chapter-item "><a href="Reference/index.html"><strong aria-hidden="true">2.</strong> Core Reference</a><a class="toggle"><div>❱</div></a></li><li><ol class="section"><li class="chapter-item "><a href="Reference/ControlFlow/index.html"><strong aria-hidden="true">2.1.</strong> Control Flow</a><a class="toggle"><div>❱</div></a></li><li><ol class="section"><li class="chapter-item "><a href="Reference/ControlFlow/ForLoop.html"><strong aria-hidden="true">2.1.1.</strong> For Loops</a></li><li class="chapter-item "><a href="Reference/ControlFlow/Repeat.html"><strong aria-hidden="true">2.1.2.</strong> the Repeat keyword</a></li><li class="chapter-item "><a href="Reference/ControlFlow/whileuntil.html"><strong aria-hidden="true">2.1.3.</strong> While Loops</a></li></ol></li><li class="chapter-item "><a href="Reference/Extending/index.html"><strong aria-hidden="true">2.2.</strong> Extending</a><a class="toggle"><div>❱</div></a></li><li><ol class="section"><li class="chapter-item "><a href="Reference/Extending/WIP_Driver.html"><strong aria-hidden="true">2.2.1.</strong> Giving gwion a new driver</a></li><li class="chapter-item "><a href="Reference/Extending/WIP_Plugins.html"><strong aria-hidden="true">2.2.2.</strong> Writing a Gwion plugin</a></li></ol></li><li class="chapter-item "><a href="Reference/Functions/index.html"><strong aria-hidden="true">2.3.</strong> Functions</a><a class="toggle"><div>❱</div></a></li><li><ol class="section"><li class="chapter-item "><a href="Reference/Functions/Lambdas.html"><strong aria-hidden="true">2.3.1.</strong> Lambda</a></li><li class="chapter-item "><a href="Reference/Functions/Variadic.html"><strong aria-hidden="true">2.3.2.</strong> Variadic functions</a></li><li class="chapter-item "><a href="Reference/Functions/Pipes.html"><strong aria-hidden="true">2.3.3.</strong> Pipes</a></li></ol></li><li class="chapter-item "><a href="Reference/Types/index.html"><strong aria-hidden="true">2.4.</strong> Types</a><a class="toggle"><div>❱</div></a></li><li><ol class="section"><li class="chapter-item "><a href="Reference/Types/Enums.html"><strong aria-hidden="true">2.4.1.</strong> Enums</a></li><li class="chapter-item "><a href="Reference/Types/Function_Pointers.html"><strong aria-hidden="true">2.4.2.</strong> Function Pointers</a></li><li class="chapter-item "><a href="Reference/Types/Primitives.html"><strong aria-hidden="true">2.4.3.</strong> Primitive types</a></li><li class="chapter-item "><a href="Reference/Types/Typedefs.html"><strong aria-hidden="true">2.4.4.</strong> Type aliases</a></li><li class="chapter-item "><a href="Reference/Types/Unions.html"><strong aria-hidden="true">2.4.5.</strong> Tagged Union</a></li></ol></li><li class="chapter-item "><a href="Reference/Preprocessor.html"><strong aria-hidden="true">2.5.</strong> Gwion Preprocessor</a><a class="toggle"><div>❱</div></a></li><li><ol class="section"><li class="chapter-item "><a href="Reference/Plugins/Analys.html"><strong aria-hidden="true">2.5.1.</strong> Analys</a></li><li class="chapter-item "><a href="Reference/Plugins/Emoji.html"><strong aria-hidden="true">2.5.2.</strong> Emoji</a></li><li class="chapter-item "><a href="Reference/Plugins/K.html"><strong aria-hidden="true">2.5.3.</strong> K</a></li><li class="chapter-item "><a href="Reference/Plugins/Lsys.html"><strong aria-hidden="true">2.5.4.</strong> Lsys</a></li><li class="chapter-item "><a href="Reference/Plugins/Math.html"><strong aria-hidden="true">2.5.5.</strong> Math</a></li><li class="chapter-item "><a href="Reference/Plugins/Modules.html"><strong aria-hidden="true">2.5.6.</strong> Modules</a></li><li class="chapter-item "><a href="Reference/Plugins/Std.html"><strong aria-hidden="true">2.5.7.</strong> Std</a></li><li class="chapter-item "><a href="Reference/Plugins/TinySF.html"><strong aria-hidden="true">2.5.8.</strong> TinySF</a></li><li class="chapter-item "><a href="Reference/Plugins/Tuple.html"><strong aria-hidden="true">2.5.9.</strong> Tuple</a></li><li class="chapter-item "><a href="Reference/Plugins/Vecx.html"><strong aria-hidden="true">2.5.10.</strong> Vecx</a></li></ol></li></ol></li><li class="chapter-item "><a href="Contributing/index.html"><strong aria-hidden="true">3.</strong> Contributing</a><a class="toggle"><div>❱</div></a></li><li><ol class="section"><li class="chapter-item "><a href="Contributing/ContributingCode.html"><strong aria-hidden="true">3.1.</strong> Contributing Code</a></li><li class="chapter-item "><a href="Contributing/ContributingDocumentation.html"><strong aria-hidden="true">3.2.</strong> Contributing Documentation</a></li><li class="chapter-item "><a href="Contributing/ContributingTranslation.html"><strong aria-hidden="true">3.3.</strong> Contributing translations</a></li><li class="chapter-item "><a href="Contributing/Plugins/index.html"><strong aria-hidden="true">3.4.</strong> Contributing Plugins</a><a class="toggle"><div>❱</div></a></li><li><ol class="section"><li class="chapter-item "><a href="Contributing/Plugins/introduction.html"><strong aria-hidden="true">3.4.1.</strong> Introduction</a></li><li class="chapter-item "><a href="Contributing/Plugins/reference.html"><strong aria-hidden="true">3.4.2.</strong> Plugin Reference</a></li><li class="chapter-item "><a href="Contributing/Plugins/credits.html"><strong aria-hidden="true">3.4.3.</strong> Credits</a></li></ol></li><li class="chapter-item "><a href="Contributing/Contributors.html"><strong aria-hidden="true">3.5.</strong> Contributors</a></li></ol></li><li class="chapter-item "><a href="Benchmarks.html">Benchmarks</a></li></ol>';
        // Set the current, active page, and reveal it if it's hidden
        let current_page = document.location.href.toString().split("#")[0];
        if (current_page.endsWith("/")) {
            current_page += "index.html";
        }
        var links = Array.prototype.slice.call(this.querySelectorAll("a"));
        var l = links.length;
        for (var i = 0; i < l; ++i) {
            var link = links[i];
            var href = link.getAttribute("href");
            if (href && !href.startsWith("#") && !/^(?:[a-z+]+:)?\/\//.test(href)) {
                link.href = path_to_root + href;
            }
            // The "index" page is supposed to alias the first chapter in the book.
            if (link.href === current_page || (i === 0 && path_to_root === "" && current_page.endsWith("/index.html"))) {
                link.classList.add("active");
                var parent = link.parentElement;
                if (parent && parent.classList.contains("chapter-item")) {
                    parent.classList.add("expanded");
                }
                while (parent) {
                    if (parent.tagName === "LI" && parent.previousElementSibling) {
                        if (parent.previousElementSibling.classList.contains("chapter-item")) {
                            parent.previousElementSibling.classList.add("expanded");
                        }
                    }
                    parent = parent.parentElement;
                }
            }
        }
        // Track and set sidebar scroll position
        this.addEventListener('click', function(e) {
            if (e.target.tagName === 'A') {
                sessionStorage.setItem('sidebar-scroll', this.scrollTop);
            }
        }, { passive: true });
        var sidebarScrollTop = sessionStorage.getItem('sidebar-scroll');
        sessionStorage.removeItem('sidebar-scroll');
        if (sidebarScrollTop) {
            // preserve sidebar scroll position when navigating via links within sidebar
            this.scrollTop = sidebarScrollTop;
        } else {
            // scroll sidebar to current active section when navigating via "next/previous chapter" buttons
            var activeSection = document.querySelector('#sidebar .active');
            if (activeSection) {
                activeSection.scrollIntoView({ block: 'center' });
            }
        }
        // Toggle buttons
        var sidebarAnchorToggles = document.querySelectorAll('#sidebar a.toggle');
        function toggleSection(ev) {
            ev.currentTarget.parentElement.classList.toggle('expanded');
        }
        Array.from(sidebarAnchorToggles).forEach(function (el) {
            el.addEventListener('click', toggleSection);
        });
    }
}
window.customElements.define("mdbook-sidebar-scrollbox", MDBookSidebarScrollbox);
