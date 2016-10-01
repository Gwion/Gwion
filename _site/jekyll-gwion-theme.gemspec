# coding: utf-8

Gem::Specification.new do |spec|
  spec.name          = "jekyll-gwion-theme"
  spec.version       = "0.1.0"
  spec.authors       = ["Jérémie Astor"]
  spec.email         = ["astor.jeremie@wanadoo.fr"]

  spec.summary       = %q{A Jekyll theme for the responsive theme for Gwion's GitHub Pages http://fennecdjay.github.io/jekyll-gwion-theme/ }
  spec.homepage      = "https://github.com/fennecdjay/jekyll-gwion-theme"
  spec.license       = "MIT"

  spec.files         = `git ls-files -z`.split("\x0").select { |f| f.match(%r{^(_layouts|_includes|_sass|LICENSE|README)/i}) }

  spec.add_development_dependency "jekyll", "~> 3.2"
  spec.add_development_dependency "bundler", "~> 1.12"
  spec.add_development_dependency "rake", "~> 10.0"
end
