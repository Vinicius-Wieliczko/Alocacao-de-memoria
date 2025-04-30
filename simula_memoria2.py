import tkinter as tk
from tkinter import messagebox

class MemoryBlock:
    def __init__(self, start, size):
        self.start = start
        self.size = size

class MemoryManager:
    def __init__(self, total_size):
        self.total_size = total_size
        self.free_blocks = [MemoryBlock(0, total_size)]
        self.allocated_blocks = []
        self.strategy = "First Fit"
        self.last_index = 0

    def set_strategy(self, strategy):
        self.strategy = strategy

    def allocate(self, size):
        if size <= 0:
            return None

        if self.strategy == "First Fit":
            block_index = next((i for i, b in enumerate(self.free_blocks) if b.size >= size), None)
        elif self.strategy == "Best Fit":
            block_index = self._find_best_fit(size)
        elif self.strategy == "Worst Fit":
            block_index = self._find_worst_fit(size)
        elif self.strategy == "Next Fit":
            block_index = self._find_next_fit(size)
        else:
            return None

        if block_index is None:
            total_free = sum(b.size for b in self.free_blocks)
            if total_free >= size:
                self.compact_memory()
                messagebox.showinfo("Compactação", "A memória foi compactada para atender à solicitação.")
                return self.allocate(size)
            else:
                return None

        block = self.free_blocks[block_index]
        allocated_block = MemoryBlock(block.start, size)
        self.allocated_blocks.append(allocated_block)

        block.start += size
        block.size -= size

        if block.size == 0:
            del self.free_blocks[block_index]

        return allocated_block.start

    def _find_best_fit(self, size):
        best_index = None
        best_size = float("inf")
        for i, block in enumerate(self.free_blocks):
            if block.size >= size and block.size < best_size:
                best_index = i
                best_size = block.size
        return best_index

    def _find_worst_fit(self, size):
        worst_index = None
        worst_size = -1
        for i, block in enumerate(self.free_blocks):
            if block.size >= size and block.size > worst_size:
                worst_index = i
                worst_size = block.size
        return worst_index

    def _find_next_fit(self, size):
        if not self.free_blocks:
            return None
        
        if self.last_index >= len(self.free_blocks):
            self.last_index = self.last_index % len(self.free_blocks)
        
        blocks_checked = 0
        i = self.last_index
        while blocks_checked < len(self.free_blocks):
            block = self.free_blocks[i]
            if block.size >= size:
                return i
            i = (i + 1) % len(self.free_blocks)
            blocks_checked += 1
        return None


    def compact_memory(self):
        self.allocated_blocks.sort(key=lambda b: b.start)
        current_address = 0
        for block in self.allocated_blocks:
            if block.start != current_address:
                block.start = current_address
            current_address += block.size
        self.free_blocks = [MemoryBlock(current_address, self.total_size - current_address)]

    def free(self, address):
        block_to_free = next((b for b in self.allocated_blocks if b.start == address), None)
        if block_to_free:
            self.allocated_blocks.remove(block_to_free)
            self.free_blocks.append(MemoryBlock(block_to_free.start, block_to_free.size))
            self.merge_free_blocks()
            if self.strategy == "Next Fit":
                for i  in range(len(self.free_blocks)):
                    if self.free_blocks[i].start == block_to_free.start and i <= self.last_index:
                        self.last_index += 1
                        break
            return True
        return False

    def merge_free_blocks(self):
        self.free_blocks.sort(key=lambda b: b.start)
        merged = []
        for block in self.free_blocks:
            if not merged:
                merged.append(block)
            else:
                last = merged[-1]
                if last.start + last.size == block.start:
                    last.size += block.size
                else:
                    merged.append(block)
        self.free_blocks = merged


    def get_free_state(self):
        return [f"Início: {b.start}, Tamanho: {b.size}" for b in self.free_blocks]

    def get_allocated_state(self):
        return [f"Início: {b.start}, Tamanho: {b.size}" for b in self.allocated_blocks]

class MemoryManagerApp:
    def __init__(self, master):
        self.master = master
        master.title("Gerenciador de Memória com Estratégias")

        self.manager = MemoryManager(100)

        self.strategy_var = tk.StringVar(value="First Fit")
        strategy_frame = tk.Frame(master)
        strategy_frame.pack(pady=5)

        tk.Label(strategy_frame, text="Estratégia de alocação:").pack(side=tk.LEFT)
        options = ["First Fit", "Best Fit", "Worst Fit", "Next Fit"]
        self.strategy_menu = tk.OptionMenu(strategy_frame, self.strategy_var, *options, command=self.change_strategy)
        self.strategy_menu.pack(side=tk.LEFT, padx=10)

        self.allocate_frame = tk.Frame(master)
        self.allocate_frame.pack(pady=5)

        tk.Label(self.allocate_frame, text="Tamanho a alocar:").grid(row=0, column=0)
        self.alloc_entry = tk.Entry(self.allocate_frame, width=5)
        self.alloc_entry.grid(row=0, column=1, padx=5)
        tk.Button(self.allocate_frame, text="Alocar", command=self.allocate_memory).grid(row=0, column=2, padx=5)

        self.free_frame = tk.Frame(master)
        self.free_frame.pack(pady=5)

        tk.Label(self.free_frame, text="Endereço para liberar:").grid(row=0, column=0)
        self.free_addr_entry = tk.Entry(self.free_frame, width=5)
        self.free_addr_entry.grid(row=0, column=1, padx=5)
        tk.Button(self.free_frame, text="Liberar", command=self.free_memory).grid(row=0, column=2, padx=5)

        self.state_frame = tk.Frame(master)
        self.state_frame.pack(pady=10)

        tk.Label(self.state_frame, text="Blocos Livres").grid(row=0, column=0)
        tk.Label(self.state_frame, text="Blocos Alocados").grid(row=0, column=1)

        self.free_listbox = tk.Listbox(self.state_frame, width=30)
        self.free_listbox.grid(row=1, column=0, padx=5)

        self.allocated_listbox = tk.Listbox(self.state_frame, width=30)
        self.allocated_listbox.grid(row=1, column=1, padx=5)

        self.update_state()

    def change_strategy(self, value):
        self.manager.set_strategy(value)

    def allocate_memory(self):
        try:
            size = int(self.alloc_entry.get().strip())
            if size <= 0:
                raise ValueError
        except ValueError:
            messagebox.showerror("Erro", "Digite um tamanho válido (inteiro positivo).")
            return

        addr = self.manager.allocate(size)
        if addr is not None:
            messagebox.showinfo("Alocado", f"Memória alocada a partir do endereço {addr}")
        else:
            messagebox.showwarning("Insuficiente", "Memória insuficiente para alocação, mesmo após compactação.")
        self.alloc_entry.delete(0, tk.END)
        self.update_state()

    def free_memory(self):
        try:
            addr = int(self.free_addr_entry.get().strip())
        except ValueError:
            messagebox.showerror("Erro", "Endereço deve ser um número inteiro.")
            return

        success = self.manager.free(addr)
        if success:
            messagebox.showinfo("Liberado", f"Memória liberada no endereço {addr}")
        else:
            messagebox.showwarning("Falha", "Endereço inválido ou não alocado.")
        self.free_addr_entry.delete(0, tk.END)
        self.update_state()

    def update_state(self):
        self.free_listbox.delete(0, tk.END)
        self.allocated_listbox.delete(0, tk.END)
        for item in self.manager.get_free_state():
            self.free_listbox.insert(tk.END, item)
        for item in self.manager.get_allocated_state():
            self.allocated_listbox.insert(tk.END, item)

if __name__ == "__main__":
    root = tk.Tk()
    app = MemoryManagerApp(root)
    root.mainloop()

