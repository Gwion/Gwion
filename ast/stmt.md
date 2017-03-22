---
layout: default
categories: [section]
title: "Statement"
target: stmt
---

<!-- count numbers of items -->
{% assign index = 0 %}
{% for post in site.pages %}
    {% if post.categories contains page.target %}
		{% assign index = index | plus: 1 %}
    {% endif %}
{% endfor %}


there are {{ index }} {{ page.title }} categories:

{% for post in site.pages %}
    {% if post.categories contains page.target %}
<li><a href="{{ site.url }}{{ post.url }}" title="{{ post.title }}"><b>{{ post.title }}</b> {{ post.description }}</a></li>
    {% endif %}
{% endfor %}
