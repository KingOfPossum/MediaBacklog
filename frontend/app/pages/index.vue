<script setup lang="ts">
import type { NavigationMenuItem } from '@nuxt/ui/components/NavigationMenu.vue'

const cards = ref([
  {
    id: 1,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co3ddc.jpg'
  },
  {
    id: 2,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co1r7l.jpg'
  },
  {
    id: 3,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co70qn.jpg'
  },
  {
    id: 4,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co5eju.jpg'
  },
  {
    id: 5,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co87hw.jpg'
  },
  {
    id: 6,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co3jky.jpg'
  },
  {
    id: 7,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co4a3e.jpg'
  },
  {
    id: 8,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co2gzc.jpg'
  },
  {
    id: 9,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/cobw10.jpg'
  },
  {
    id: 10,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co55n7.jpg'
  },
  {
    id: 11,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co3koh.jpg'
  },
  {
    id: 12,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/cob1t0.jpg'
  },
  {
    id: 13,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co4990.jpg'
  },
  {
    id: 14,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/cob9xh.jpg'
  },
  {
    id: 15,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co3p5n.jpg'
  },
  {
    id: 16,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co20mh.jpg'
  },
  {
    id: 17,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co2m9q.jpg'
  },
  {
    id: 18,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co2str.jpg'
  },
  {
    id: 19,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co6ya8.jpg'
  },
  {
    id: 20,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co6lxr.jpg'
  },
  {
    id: 21,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/coakuz.jpg'
  },
  {
    id: 22,
    img: 'https://images.igdb.com/igdb/image/upload/t_1080p/co5uzk.jpg'
  }
])

const items = ref<NavigationMenuItem[]>([
  {
    label: 'Home',
    icon: 'i-lucide-home'
  },
  {
    label: 'Games',
    icon: 'i-lucide-gamepad-2'
  },
  {
    label: 'Books',
    icon: 'i-lucide-book'
  }
])
</script>

<template>
  <div class="h-screen flex flex-col">
    <!--
    -->
    <UHeader :ui="{ container: 'px-4!' }" />

    <div class="flex flex-1 min-h-0 overflow-hidden">
      <USidebar
        rail
        close
        collapsible="icon"
        side="left"
        class="top-64px!"
        :ui="{ container: 'px-4!' }"
      >
        <UNavigationMenu orientation="vertical" :items="items" />
      </USidebar>

      <div class="grid grid-cols-3 md:grid-cols-4 lg:grid-cols-5 xl:grid-cols-6 2xl:grid-cols-7 gap-5 p-4 overflow-y-auto flex-1 content-start">
        <img
          v-for="card in cards"
          :key="card.id"
          :src="card.img"
          alt="i"
          class="rounded-2xl object-cover w-full aspect-3/4 hover:scale-105 transition-transform"
        />
      </div>
    </div>
  </div>
</template>
