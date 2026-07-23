<script setup>
  const props = defineProps({
    console: {
      type: String,
      required: true
    },
    size: {
      type: String,
      default: '32',
      required: false
    }
  })
</script>

<template>
  <div>
    <img
      :src="`/platformIconsSprites/${props.console}.svg`"
      :width="size" :height="size"
      class="
        hover:scale-250
        hover:delay-400
        transition-transform
        ease-in-out
        duration-500
      "
      :title="console"
    />
  </div>

</template>
