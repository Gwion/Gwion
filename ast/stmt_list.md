---
layout: default
categories: [section]
title: "Statement list"
---

<!-- count numbers of items -->
{% assign index = 0 %}
{% for post in site.pages %}
    {% if post.categories contains 'stmt' %}
		{% assign index = index | plus: 1 %}
    {% endif %}
{% endfor %}


there are {{ index }} {{ page.title }} categories:

{% for post in site.pages %}
    {% if post.categories contains 'stmt' %}
      <h1 class="entry-title"><a href="{{ site.url }}{{ post.url }}" rel="bookmark" title="{{ 
post.title }}" itemprop="url">{{ post.title}}</a></h1>
    {% endif %}
{% endfor %}
