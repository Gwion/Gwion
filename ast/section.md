---
layout: default
categories: [ast]
title: "Section"
target: section
---

<!-- count numbers of items -->
{% assign index = 0 %}
{% for post in site.pages %}
    {% if post.categories contains page.target %}
        {% assign index = index | plus: 1 %}
    {% endif %}
{% endfor %}

{{ index }}
{{ page.target }}

{% for post in site.pages %}
    {% if post.categories contains page.target %}
      <h1 class="entry-title"><a href="{{ site.url }}{{ post.url }}" rel="bookmark" title="{{post.title }}" itemprop="url">{{ post.title}}</a></h1>
    {% endif %}
{% endfor %}


# there are three section types:
  * Statement list
  * Function definition
  * Class definition
