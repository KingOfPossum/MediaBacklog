<script setup lang="ts">
  const cards = ref([
    {
      title: 'Icons',
      description: 'Desadascription of card!!!',
      icon: 'i-simple-icons-tailwindcss',
      to: '',
      orientation: 'vertical',
    },
    {
      title: 'Icons',
      description: 'Desadascription of card!!!',
      icon: 'i-simple-icons-tailwindcss',
      to: '',
      orientation: 'horizontal',
    },
    {
      title: 'Icons',
      description: 'Desadascription of card!!!',
      icon: 'i-simple-icons-tailwindcss',
      to: '',
      orientation: 'horizontal',
    },
    {
      title: 'Icons',
      description: 'Desadascription of card!!!',
      icon: 'i-simple-icons-tailwindcss',
      to: '',
      orientation: 'horizontal',
    }
  ])
</script>

<template>
  <div>
    <div class="flex flex-col flex-1">
      <UHeader :ui="{container: 'px-4!'}">

      </UHeader>

      <div class="flex flex-1 min-h-0">
        <USidebar rail :ui="{container: 'px-4!'}">
        </USidebar>

        <div class="flex-1 p-4">
          
        </div>
      </div>
    </div>
  </div>
</template>
