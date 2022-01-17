<script>
  let text = `s 100 100 100 100`;
  const step = 100;

  async function sendCommand(command) {
    command = command ?? text;

    console.log(command);

    const res = await fetch("http://192.168.86.22/post", {
      method: "POST",
      mode: "no-cors",
      headers: {
        "Content-Type": "text/plain",
      },
      body: text,
    });
  }
</script>

<main>
  <h1>Motors</h1>
  <div class="buttons-grid">
    <p>Base</p>
    <button> anti </button>
    <button> clockwise </button>

    <p>Shoulder</p>
    <button> anti </button>
    <button> clockwise </button>

    <p>Elbow</p>
    <button> anti </button>
    <button> clockwise </button>

    <p>Wrist</p>
    <button> anti </button>
    <button> clockwise </button>
  </div>

  <h1>Axes</h1>
  <div class="circle-cont">
    <div class="group-title">Z Axis</div>
    <button>↑</button>
    <button>↑↑</button>
    <button>↑↑↑</button>

    <button>↓</button>
    <button>↓↓</button>
    <button>↓↓↓</button>

    <div class="group-title">X Axis</div>
    <button>→</button>
    <button>→→</button>
    <button>→→→</button>

    <button>←</button>
    <button>←←</button>
    <button>←←←</button>

    <div class="group-title">Y Axis</div>
    <button>→</button>
    <button>→→</button>
    <button>→→→</button>

    <button>←</button>
    <button>←←</button>
    <button>←←←</button>
  </div>

  <h1>EHCODE</h1>
  <div class="g-code">
    <textarea
      id="textarea"
      rows="10"
      cols="40"
      placeholder="Your wish is my command"
      bind:value={text}
    />
    <button class="send" on:click={sendCommand}>Send</button>
  </div>
</main>

<style>
  main {
    text-align: center;
    padding: 1em;
    max-width: 400px;
    margin: 0 auto;
  }

  .buttons-grid {
    display: grid;
    grid-template-columns: repeat(3, 1fr);
    grid-gap: 0em 1em;
  }

  .circle-cont {
    display: grid;
    grid-template-columns: repeat(3, 1fr);
    grid-gap: 0em 1em;
  }

  .group-title {
    grid-column: 1/-1;
    margin: 1em 0 0.5em 0;
  }

  button {
    border-radius: 5px;
    box-shadow: 1px 1px 2px #aaa;
    border: none;
    padding: 0.5em;
    font-size: 1em;
    font-weight: bold;
    background-color: #66aa88;
    color: #ffffff;
    cursor: pointer;
  }

  button:hover {
    background-color: #88bb99;
  }

  button:active {
    background-color: #449977;
    box-shadow: none;
  }

  .send {
    width: 50%;
  }
</style>
